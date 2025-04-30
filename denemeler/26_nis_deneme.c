/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   26_nis_deneme.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:55:27 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/30 17:10:48 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <stdbool.h>




static void	fill_args(char **args, int *ac, line_t *node)
{
    while (node)
    {
        if (node->type == REDIRECT_IN || node->type == REDIRECT_OUT
            || node->type == HEREDOC_IN || node->type == HEREDOC_OUT)
        {
            node = node->next;
            if (node)
                node = node->next;
            continue;
        }
        if (node->type == WORD)
            args[(*ac)++] = strdup(node->value);
        node = node->next;
    }
    args[*ac] = NULL;
}

static char	*get_exec_path(char **args, minishell_t *ms)
{
    char	*exec_path;

    exec_path = NULL;
    if (args[0] && access(args[0], X_OK) == 0)
        exec_path = strdup(args[0]);
    else if (args[0])
        exec_path = find(args[0], ms->envp, ms);
    return (exec_path);
}

static void	free_args(char **args, int ac)
{
    int	i = 0;
    while (i < ac)
        free(args[i++]);
    free(args);
}

static void	child_exec(line_t *cur, minishell_t *ms, int heredoc_fd)
{
    char	**args = malloc(sizeof(char *) * 100);
    int		ac = 0;
    char	*exec_path;

    fill_args(args, &ac, cur);

    if(ac == 0)//<< eof durumunda ana döngüye geri dönüyor. buradaki free kısmına dikkat et 
    {
        free(args);
        exit(1);
    }

    apply_redirections(cur, heredoc_fd);
    exec_path = get_exec_path(args, ms);
    if (!exec_path)
    {
        fprintf(stderr, "Command not found: %s\n", args[0]);
        free_args(args, ac);
        exit(127);
    }
    execve(exec_path, args, ms->envp);
    perror("execve");//perror
    free_args(args, ac);
    free(exec_path);
    exit(EXIT_FAILURE);
}

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
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, delimiter) == 0)
            break;
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
    }
    free(line);
    close(pipefd[1]);
}

static void	redir_in(line_t *cur)
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

static void	redir_out(line_t *cur, int append)
{
    int	fd;
    if (append)
        fd = open(cur->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(cur->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
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
                || cur->type == HEREDOC_OUT) && cur->next)
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
        dup2(heredoc_fd, STDIN_FILENO);
        close(heredoc_fd);
    }
}


static int	handle_heredocs(line_t *line)
{
    line_t *tmp = line;
    int last_pipe[2] = {-1, -1};
    int prev_read = -1;

    while (tmp)
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

            prev_read = pipefd[0]; 

        }
        tmp = tmp->next;
    }
    return prev_read;
}

// Her komut için heredoc fd'lerini saklayacağımız bir dizi
typedef struct s_heredoc_map {
    int fd;
} heredoc_map_t;

// Pipeline'daki tüm heredoc'ları baştan işle
static heredoc_map_t *collect_all_heredocs(line_t **mini_lines, int cmd_count) {
    heredoc_map_t *heredocs = malloc(sizeof(heredoc_map_t) * cmd_count);
    for (int i = 0; i < cmd_count; i++) {
        heredocs[i].fd = handle_heredocs(mini_lines[i]);
    }
    return heredocs;
}

void execute_pipeline(minishell_t *ms)
{
    int cmd_count = 0;
    int i = 0;
    pid_t *pids;
    int pipefd[2];
    int prev_fd = -1;

    while (ms->mini_lines[cmd_count])
        cmd_count++;

    // Pipeline başlamadan önce tüm heredoc'ları sırayla işle
    heredoc_map_t *heredocs = collect_all_heredocs(ms->mini_lines, cmd_count);

    pids = malloc(sizeof(pid_t) * cmd_count);
    while (i < cmd_count)
    {
        int heredoc_fd = heredocs[i].fd;
        if (i < cmd_count - 1)
            pipe(pipefd);
        pids[i] = fork();
        if (pids[i] == 0)
        {
            if (i > 0)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (i < cmd_count - 1)
            {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
            apply_redirections(ms->mini_lines[i], heredoc_fd);
            child_exec(ms->mini_lines[i], ms, -1);
        }
        if (i > 0)
            close(prev_fd);
        if (i < cmd_count - 1)
        {
            close(pipefd[1]);
            prev_fd = pipefd[0];
        }
        if (heredoc_fd != -1)
            close(heredoc_fd);
        i++;
    }
    i = 0;
    while (i < cmd_count)
        waitpid(pids[i++], NULL, 0);
    free(pids);
    free(heredocs);
}

void	priority(minishell_t *ms)
{
    int		max_priority;
    int		i;
    line_t	*current;

    i = 0;
    while (ms->mini_lines[i])
    {
        max_priority = 0;
        current = ms->mini_lines[i];
        while (current != NULL)
        {
            if (current->value != NULL)
            {
                if (ft_strncmp("<<", current->value, 2) == 0
                    || ft_strncmp(">>", current->value, 2) == 0)
                {
                    max_priority = 2;
                    break;
                }
                else if ((ft_strncmp("<", current->value, 1) == 0
                        || ft_strncmp(">", current->value, 1) == 0)
                    && max_priority < 1)
                    max_priority = 1;
            }
            current = current->next;
        }
        if (ms->mini_lines[i] != NULL)
            ms->mini_lines[i]->priority = max_priority;
        i++;
    }
}

