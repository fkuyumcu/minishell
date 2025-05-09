/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:02:09 by yalp              #+#    #+#             */
/*   Updated: 2025/05/07 17:07:37 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wait_all(pid_t *pids, int cmd_count)
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_count)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (WIFEXITED(status))
				g_code = WEXITSTATUS(status);
		}
		i++;
	}
}

int	cmd_counter(t_line **mini_lines)
{
	int	count;

	count = 0;
	while (mini_lines[count])
		count++;
	return (count);
}

void	setup_pipe(int i, int cmd_count, int *pipefd, int **curr_pipe)
{
	*curr_pipe = NULL;
	if (i < cmd_count - 1)
	{
		if (pipe(pipefd) == -1)
			exit(1);
		*curr_pipe = pipefd;
	}
}
