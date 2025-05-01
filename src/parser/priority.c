/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:55:27 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/01 15:06:21 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/wait.h> // waitpid ve makrolar için

extern int global_code; // global değişkenin tanımlı olduğundan emin ol

void	execute_pipeline(minishell_t *ms)
{
	int		cmd_count;
	pid_t	*pids;
	int		i;
	int		pipefd[2];
	int		prev_fd;
	int		heredoc_fd;

	i = 0;
	cmd_count = 0;
	prev_fd = -1;
	while (ms->mini_lines[cmd_count])
		cmd_count++;
	pids = malloc(sizeof(pid_t) * cmd_count);
    
	while (i < cmd_count)                    
	{
		heredoc_fd = handle_heredocs(ms->mini_lines[i], ms);
		if (i < cmd_count - 1)
			pipe(pipefd);

		//BUILTINLERIN ÖNCELİKLERİ
		if (is_builtin(ms->mini_lines[i]) == 1)
		{
			int original_stdout = dup(STDOUT_FILENO);
			int original_stdin = dup(STDIN_FILENO);
			
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
			
			apply_redirections(ms->mini_lines[i], heredoc_fd, ms);
			
			try_builtins(ms->mini_lines[i], ms);
			
			dup2(original_stdout, STDOUT_FILENO);
			dup2(original_stdin, STDIN_FILENO);
			close(original_stdout);
			close(original_stdin);
		}
		else if (is_builtin(ms->mini_lines[i]) == 0)
		{
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
				apply_redirections(ms->mini_lines[i], heredoc_fd, ms);
				child_exec(ms->mini_lines[i], ms, -1);
			}
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
	int status;
    while (i < cmd_count)
    {
        status = 0;
        waitpid(pids[i], &status, 0);
        if (i == cmd_count - 1)
        {
            if (WIFEXITED(status))
                global_code = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                global_code = 128 + WTERMSIG(status);
            else
                global_code = 1;
        }
        i++;
    }
    free(pids);
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
				if (ft_strncmp("<<", current->value, 2) == 0 || ft_strncmp(">>",
						current->value, 2) == 0)
				{
					max_priority = 2;
					break ;
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
