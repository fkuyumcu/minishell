/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:02:13 by yalp              #+#    #+#             */
/*   Updated: 2025/05/09 09:01:57 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_pids(t_minishell *ms, int cmd_count)
{
	int	j;

	ms->pids = malloc(sizeof(pid_t) * cmd_count);
	if (!ms->pids)
		return ;
	j = 0;
	while (j < cmd_count)
	{
		ms->pids[j] = -1;
		j++;
	}
}

void	bi_child(t_minishell *ms, int i, int *pipefd, int prev_fd)
{
	int	heredoc_fd;

	heredoc_fd = handle_heredocs(ms->mini_lines[i], ms);
	if (i > 0)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (pipefd && i >= 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	apply_redirections(ms->mini_lines[i], heredoc_fd);
	try_builtins(ms->mini_lines[i], ms);
	check_and_free(ms, ms->mini_lines[i], 0);
	exit(g_code);
}

void	bi_parent(t_minishell *ms, int i, int heredoc_fd)
{
	int	original_stdout;
	int	original_stdin;

	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	apply_redirections(ms->mini_lines[i], heredoc_fd);
	try_builtins(ms->mini_lines[i], ms);
	dup2(original_stdout, STDOUT_FILENO);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdout);
	close(original_stdin);
}

void	handle_builtin(t_minishell *ms, int i, int *pipefd, int prev_fd)
{
	pid_t	pid;

	if ((pipefd && i >= 0) || prev_fd != -1)
	{
		pid = fork();
		if (pid == 0)
			bi_child(ms, i, pipefd, prev_fd);
		else
			ms->pids[i] = pid;
	}
	else
		bi_parent(ms, i, handle_heredocs(ms->mini_lines[i], ms));
}

void	handle_path_exec(t_minishell *ms, int i, int *pipefd, int prev_fd)
{
	int	heredoc_fd;

	heredoc_fd = handle_heredocs(ms->mini_lines[i], ms);
	ms->pids[i] = fork();
	if (ms->pids[i] == 0)
	{
		if (i > 0)
		{
			dup2(prev_fd, STDIN_FILENO);
			close(prev_fd);
		}
		if (pipefd && i >= 0)
		{
			close(pipefd[0]);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
		}
		apply_redirections(ms->mini_lines[i], heredoc_fd);
		child_exec(ms->mini_lines[i], ms, -1);
		exit(1);
	}
	if (heredoc_fd != -1)
		close(heredoc_fd);
}
