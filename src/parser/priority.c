/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:55:27 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/07 17:05:22 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_code;

static void	manage_pipe(int i, int cmd_count, int *pipefd, int *prev_fd)
{
	if (i > 0)
		close(*prev_fd);
	if (i < cmd_count - 1)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
}

static void	execute_loop(t_minishell *ms, int cmd_count)
{
	int	i;
	int	pipefd[2];
	int	prev_fd;
	int	*curr_pipe;

	i = 0;
	prev_fd = -1;
	while (i < cmd_count)
	{
		setup_pipe(i, cmd_count, pipefd, &curr_pipe);
		if (is_builtin(ms->mini_lines[i]))
			handle_builtin(ms, i, curr_pipe, prev_fd);
		else
			handle_path_exec(ms, i, curr_pipe, prev_fd);
		manage_pipe(i, cmd_count, pipefd, &prev_fd);
		i++;
	}
}

void	execute_pipeline(t_minishell *ms)
{
	int	cmd_count;

	cmd_count = cmd_counter(ms->mini_lines);
	if (cmd_count <= 0)
		return ;
	init_pids(ms, cmd_count);
	execute_loop(ms, cmd_count);
	wait_all(ms->pids, cmd_count);
	free(ms->pids);
	ms->pids = NULL;
}

static int	get_max_priority(t_line *line)
{
	int	max_priority;

	max_priority = 0;
	while (line != NULL)
	{
		if (line->value != NULL)
		{
			if (ft_strncmp("<<", line->value, 2) == 0 || ft_strncmp(">>",
					line->value, 2) == 0)
				return (2);
			else if ((ft_strncmp("<", line->value, 1) == 0 || ft_strncmp(">",
						line->value, 1) == 0) && max_priority < 1)
				max_priority = 1;
		}
		line = line->next;
	}
	return (max_priority);
}

void	priority(t_minishell *ms)
{
	int	i;
	int	max_priority;

	i = 0;
	while (ms->mini_lines[i])
	{
		max_priority = get_max_priority(ms->mini_lines[i]);
		ms->mini_lines[i]->priority = max_priority;
		i++;
	}
}
