#include "../minishell.h"

#include <fcntl.h>

void apply_redirections(line_t *cmd)
{
    line_t *current = cmd;

    while (current)
    {
        if ((current->type == REDIRECT_IN || current->type == REDIRECT_OUT ||
             current->type == HEREDOC_IN || current->type == HEREDOC_OUT) &&
            current->next)
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
            else if (current->type == HEREDOC_IN) // << (heredoc)
            {
                // Heredoc özel bir durum, genellikle geçici bir dosyada tutulur.
                // Burada örnek olarak `/tmp/heredoc` kullanıyoruz.
                fd = open("/tmp/.heredoc", O_RDONLY);
                if (fd == -1)
                {
                    perror("open <<");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            current = current->next; // argümanı atla
        }

        current = current->next;
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

            if (pids[i] == 0)
            {
                // Önce yönlendirmeleri uygula
                apply_redirections(current);

                // Sonra pipe'ları ayarla
                if (prev_pipe[0] != -1)
                {
                    dup2(prev_pipe[0], STDIN_FILENO);
                    close(prev_pipe[0]);
                    close(prev_pipe[1]);
                }

                if (i < cmd_count - 1)
                {
                    close(curr_pipe[0]);
                    dup2(curr_pipe[1], STDOUT_FILENO);
                    close(curr_pipe[1]);
                }

                char **args = malloc(sizeof(char *) * 100);
                int arg_count = 0;
                line_t *tmp = current;

                while (tmp)
                {
                    if (tmp->type != REDIRECT_IN &&
                        tmp->type != REDIRECT_OUT &&
                        tmp->type != HEREDOC_IN &&
                        tmp->type != HEREDOC_OUT)
                    {
                        args[arg_count++] = tmp->value;
                    }
                    tmp = tmp->next;
                }
                args[arg_count] = NULL;

                execvp(args[0], args);
                perror("execvp");
                free(args);
                exit(EXIT_FAILURE);
            }
            else
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
