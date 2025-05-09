/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:54:10 by yalp              #+#    #+#             */
/*   Updated: 2025/05/07 15:56:20 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_lines(t_minishell *ms, t_line *cur)
{
	int	i;

	if (!ms->mini_lines)
		return ;
	i = 0;
	while (ms->mini_lines[i])
	{
		if (ms->mini_lines[i] != cur)
			free_line(ms->mini_lines[i]);
		i++;
	}
	free(ms->mini_lines);
	ms->mini_lines = NULL;
}

void	free_args_exec(char **args, int ac, char *exec_path)
{
	if (args)
		free_args(args, ac);
	if (exec_path)
	{
		free(exec_path);
		exec_path = NULL;
	}
}

void	free_main_line(t_minishell *ms, t_line *cur)
{
	if (ms->line && ms->line != cur)
		free_line(ms->line);
	ms->line = NULL;
}

void	free_pids(t_minishell *ms)
{
	if (ms->pids)
	{
		free(ms->pids);
		ms->pids = NULL;
	}
}

void	check_and_free(t_minishell *ms, t_line *cur, int update_args)
{
	if (ms && update_args)
	{
		if (ms->exec_args)
		{
			free_args_exec(ms->exec_args, ms->exec_ac, ms->exec_path);
			ms->exec_args = NULL;
			ms->exec_path = NULL;
		}
	}
	if (ms)
	{
		if (ms->temp)
		{
			free(ms->temp);
			ms->temp = NULL;
		}
		free_tokens_end(ms);
		free_main_line(ms, cur);
		free_lines(ms, cur);
		free_pids(ms);
		free_envp_ms(ms);
	}
	if (cur && cur->value && (!ms || (ms && !ms->mini_lines)))
		free_line(cur);
	close_fds();
}
