/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:55:27 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/23 15:15:46 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <stdbool.h>

#include <fcntl.h>
#include <stdbool.h>

void apply_heredoc_pipe(line_t *heredoc_node, int pipefd[2])
{
    if (!heredoc_node || !heredoc_node->next || !heredoc_node->next->value)
    {
        fprintf(stderr, "Heredoc error: Invalid delimiter\n");
        exit(EXIT_FAILURE);
    }

    char *delimiter = heredoc_node->next->value;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while (1)
    {
        write(STDOUT_FILENO, "> ", 2);
        read = getline(&line, &len, stdin);
        
        if (read == -1)
        {
            if (feof(stdin))
            {
                fprintf(stderr, "warning: here-document delimited by end-of-file\n");
                break;
            }
            perror("getline");
            free(line);
            exit(EXIT_FAILURE);
        }

        // Remove newline if present
        if (read > 0 && line[read - 1] == '\n')
            line[read - 1] = '\0';

        // Compare with delimiter
        if (strcmp(line, delimiter) == 0)
            break;

        // Write original line with newline
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
    }

    free(line);
    close(pipefd[1]);
}

void apply_redirections(line_t *cmd, int heredoc_fd)
{
    line_t *current = cmd;

    while (current)
    {
        if ((current->type == REDIRECT_IN || current->type == REDIRECT_OUT ||
             current->type == HEREDOC_OUT) && current->next)
        {
            int fd;

            if (current->type == REDIRECT_IN) // <
            {
                fd = open(current->next->value, O_RDONLY);
                if (fd == -1)
                {
                    perror("open <");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            else if (current->type == REDIRECT_OUT) // >
            {
                fd = open(current->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1)
                {
                    perror("open >");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (current->type == HEREDOC_OUT) // >>
            {
                fd = open(current->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd == -1)
                {
                    perror("open >>");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            current = current->next;
        }

        current = current->next;
    }

    // heredoc yönlendirmesi ayrı
    if (heredoc_fd != -1)
    {
        dup2(heredoc_fd, STDIN_FILENO);
        close(heredoc_fd);
    }
}

void execute_pipeline(minishell_t *ms)
{
    int cmd_count = 0;
    int i, priority_level;
    pid_t *pids;
    int prev_pipe[2] = {-1, -1};
    int curr_pipe[2] = {-1, -1};

    while (ms->mini_lines[cmd_count])
        cmd_count++;

    pids = malloc(sizeof(pid_t) * cmd_count);

    for (priority_level = 2; priority_level >= 0; priority_level--)
    {
        for (i = 0; i < cmd_count; i++)
        {
            line_t *current = ms->mini_lines[i];

            if (current->priority != priority_level)
                continue;

            // heredoc kontrolü
            int heredoc_pipe[2] = {-1, -1};
            bool has_heredoc = false;
            line_t *tmp = current;

            while (tmp)
            {
                if (tmp->type == HEREDOC_IN)
                {
                    has_heredoc = true;
                    if (pipe(heredoc_pipe) == -1)
                    {
                        perror("pipe heredoc");
                        exit(EXIT_FAILURE);
                    }
                    apply_heredoc_pipe(tmp, heredoc_pipe); // kullanıcıdan input al
                    break;
                }
                tmp = tmp->next;
            }

            if (i < cmd_count - 1)
            {
                if (pipe(curr_pipe) == -1)
                {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }
            }

            pids[i] = fork();
            if (pids[i] == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (pids[i] == 0) // CHILD
            {


                char **args = malloc(sizeof(char *) * 100);
                int arg_count = 0;
                line_t *node = current;
        
                // Düzeltilmiş kısım - komut ve argümanları doğru şekilde ayır
                while (node)
                {
                    // Yönlendirme operatörleri ve argümanlarını atla
                    if (node->type == REDIRECT_IN || node->type == REDIRECT_OUT ||
                        node->type == HEREDOC_IN || node->type == HEREDOC_OUT)
                    {
                        node = node->next; // Yönlendirme operatörünü atla
                        if (node) node = node->next; // Yönlendirme argümanını atla
                        continue;
                    }
                    
                    if (node->type == WORD)
                    {
                        args[arg_count++] = strdup(node->value);
                    }
                    node = node->next;
                }
                args[arg_count] = NULL;
        
                // Yönlendirmeleri uygula
                apply_redirections(current, has_heredoc ? heredoc_pipe[0] : -1);
        
                // Komutu çalıştır
                execvp(args[0], args);
                perror("execvp");
                for (int j = 0; j < arg_count; j++)
                    free(args[j]);
                free(args);
                exit(EXIT_FAILURE);
            }
            else // PARENT
            {
                if (prev_pipe[0] != -1)
                {
                    close(prev_pipe[0]);
                    close(prev_pipe[1]);
                }

                if (i < cmd_count - 1)
                {
                    prev_pipe[0] = curr_pipe[0];
                    prev_pipe[1] = curr_pipe[1];
                }

                if (has_heredoc)
                {
                    close(heredoc_pipe[0]); // child kullanacak
                }
            }
        }

        for (i = 0; i < cmd_count; i++)
        {
            if (ms->mini_lines[i]->priority == priority_level)
            {
                int status;
                waitpid(pids[i], &status, 0);
            }
        }
    }

    free(pids);
}

void priority(minishell_t *ms)
{   
    int max_priority;
    int i;
    line_t *current;

    i = 0;
    
    while(ms->mini_lines[i])
    {
        max_priority = 0;
        current = ms->mini_lines[i];
        while(current != NULL)
        {
            if (current->value != NULL)
            {
                if (ft_strncmp("<<", current->value, 2) == 0 || 
                    ft_strncmp(">>", current->value, 2) == 0)
                {
                    max_priority = 2;
                    break; 
                }
                else if ((ft_strncmp("<", current->value, 1) == 0 || 
                         ft_strncmp(">", current->value, 1) == 0) &&
                         max_priority < 1)
                {
                    max_priority = 1;
                }
            }
            current = current->next;
        }
        if (ms->mini_lines[i] != NULL)
        {
            ms->mini_lines[i]->priority = max_priority;
        }
        i++;
    }
}


