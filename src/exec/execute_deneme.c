/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_deneme.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 18:23:14 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/14 18:23:42 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

void execute_pipeline(minishell_t *ms)
{
    int i = 0;
    int cmd_count = 0;
    int pipes[2][2];
    pid_t *pids;


    while (ms->mini_lines[cmd_count]) cmd_count++;
    pids = malloc(sizeof(pid_t) * cmd_count);


    for (i = 0; i < cmd_count; i++)
    {
        if (i < cmd_count - 1)
        {
            if (pipe(pipes[i % 2]) == -1)
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

        if (pids[i] == 0)
        {

            if (i > 0)
            {
                dup2(pipes[(i + 1) % 2][0], STDIN_FILENO);
                close(pipes[(i + 1) % 2][0]);
                close(pipes[(i + 1) % 2][1]);
            }

            if (i < cmd_count - 1)
            {
                dup2(pipes[i % 2][1], STDOUT_FILENO);
                close(pipes[i % 2][0]);
                close(pipes[i % 2][1]);
            }

            line_t *curr = ms->mini_lines[i];
            char **args = malloc(sizeof(char *) * 100);
            int arg_count = 0;

            while (curr)
            {
                if (curr->value[0] == '>' || curr->value[0] == '<')
                {
                    // Redirection işle
                    if (strcmp(curr->value, ">") == 0)
                    {
                        int fd = open(curr->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                        break;
                    }

                }
                else
                {
                    args[arg_count++] = curr->value;
                }
                curr = curr->next;
            }
            args[arg_count] = NULL;

            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }


        if (i > 0)
        {
            close(pipes[(i + 1) % 2][0]);
            close(pipes[(i + 1) % 2][1]);
        }
    }


    int status;
    for (i = 0; i < cmd_count; i++)
    {
        waitpid(pids[i], &status, 0);
    }

    free(pids);
}