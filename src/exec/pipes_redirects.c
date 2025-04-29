/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:35:38 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/29 12:47:31 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



void	apply_heredoc_pipe(line_t *heredoc_node, int pipefd[2])
{
    char	*delimiter;
    char	*line = NULL;
    size_t	len = 0;
    ssize_t	read;

    if (!heredoc_node || !heredoc_node->next || !heredoc_node->next->value)
    {
        fprintf(stderr, "Heredoc error: Invalid delimiter\n");
        exit(EXIT_FAILURE);
    }
    delimiter = heredoc_node->next->value;
    while (1)
    {
        write(STDOUT_FILENO, "> ", 2);
        read = getline(&line, &len, stdin);
        if (read == -1)
            break;
        line[strcspn(line, "\n")] = '\0';//strcsmp
        if (strcmp(line, delimiter) == 0)//strcmp
            break;
        write(pipefd[1], line, ft_strlen(line));
        write(pipefd[1], "\n", 1);
    }
    free(line);
    close(pipefd[1]);
}

void	redir_in(line_t *cur)
{
    int	fd = open(cur->next->value, O_RDONLY);
    if (fd == -1)
    {
        perror("open <");
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
        //perror(append ? "open >>" : "open >");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void	apply_redirections(line_t *cmd, int heredoc_fd)
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


int	handle_heredocs(line_t *line)
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
                perror("pipe heredoc");
                exit(EXIT_FAILURE);
            }
            apply_heredoc_pipe(tmp, pipefd);

            if (prev_read != -1)
                close(prev_read); 

            prev_read = pipefd[0];//yalnızca son heredocun yazma ucunu saklar

        }
        tmp = tmp->next;
    }
    return prev_read;
}