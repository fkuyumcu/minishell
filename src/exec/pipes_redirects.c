/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:35:38 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/29 18:58:02 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_redirect(line_t *cur)
{
    line_t *head;
    int    has_redirect;

    head = cur;
    has_redirect = 0;
    while (cur->next != NULL)
    {
        // Heredoc input'u varsa kontrol et
        if (cur->type == HEREDOC_IN)
        {
            // Eğer sonrasında redirect varsa yazmayı engelle
            line_t *temp = cur;
            while (temp->next)
            {
                if (temp->type == REDIRECT_OUT || temp->type == HEREDOC_OUT)
                {
                    if (!temp->next || temp->next->type != WORD)
                    {
                        cur = head;
                        return (0);
                    }
                }
                temp = temp->next;
            }
        }
        // Normal redirect kontrolü
        if (cur->type == HEREDOC_OUT || cur->type == REDIRECT_OUT)
        {
            has_redirect = 1;
            if (!cur->next || cur->next->type != WORD)
            {
                cur = head;
                return (0);
            }
        }
        cur = cur->next;
    }
    cur = head;
    return (1);
}

int	check_redirect_after_heredoc(line_t *heredoc_node)
{
    line_t *cur = heredoc_node->next;
    while (cur)
    {
        if ((cur->type == REDIRECT_OUT || cur->type == HEREDOC_OUT))
        {
            if (!cur->next || cur->next->type != WORD)
                return (0); // redirect var ve dosya adı yok
        }
        cur = cur->next;
    }
    return (1); // sorun yok
}

void	apply_heredoc_pipe(line_t *heredoc_node, int pipefd[2], minishell_t *ms)
{
    char	*delimiter;
    char	*line;

    if (!heredoc_node || !heredoc_node->next || !heredoc_node->next->value)
    {
        printf("Invalid Delimeter!\n");
        return;
    }

    // heredoc'tan sonra redirect olup olmadığını ve dosya adını kontrol et
    int can_write = check_redirect_after_heredoc(heredoc_node);

    delimiter = heredoc_node->next->value;
    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
        {
            free(line);
            break;
        }
        if (can_write == 1)
        {
            write(pipefd[1], line, ft_strlen(line));
            write(pipefd[1], "\n", 1);
        }
        free(line);
    }
    close(pipefd[1]);
}

void	redir_in(line_t *cur)
{
    int	fd = open(cur->next->value, O_RDONLY);
    if (fd == -1)
    {
        printf("error <");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void	redir_out(line_t *cur, int append)
{
    int	fd;
    if (append)
        fd = open(cur->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(cur->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        printf("file descriptor error");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void	apply_redirections(line_t *cmd, int heredoc_fd, minishell_t *ms)
{
    line_t	*cur = cmd;
    while (cur)
    {
        if ((cur->type == REDIRECT_IN || cur->type == REDIRECT_OUT
                || cur->type == HEREDOC_OUT) && cur->next)//miniline'ın içinde her bir komutun içinde dolaş, son komut olmadığı sürece yönlendirmeleri yap
        {
            if (cur->type == REDIRECT_IN)
                redir_in(cur);
            else
                redir_out(cur, cur->type == HEREDOC_OUT);
            cur = cur->next;
        }
        cur = cur->next;
    }
    if (heredoc_fd != -1)
    {
        dup2(heredoc_fd, STDIN_FILENO);//heredocun girişi için stdin ile yerini değiştir
        close(heredoc_fd);//okuma ucunu kapat
    }
}


int	handle_heredocs(line_t *line, minishell_t *ms)
{
    line_t *tmp = line;
    int last_pipe[2] = {-1, -1};
    int prev_read = -1;

    while (tmp)//bütün heredoclar içinde dolaşıyori heredoc bulduğu anda yeni pipe açıyor
    {
        if (tmp->type == HEREDOC_IN)
        {
            int pipefd[2];
            if (pipe(pipefd) == -1)
            {
                printf("pipe error");
                exit(EXIT_FAILURE);
            }
            apply_heredoc_pipe(tmp, pipefd, ms);

            if (prev_read != -1)
                close(prev_read); 

            prev_read = pipefd[0];//yalnızca son heredocun yazma ucunu saklar

        }
        tmp = tmp->next;
    }
    return prev_read;
}