/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_procs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:32:51 by fkuyumcu          #+#             */
/*   Updated: 2025/05/01 17:44:20 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_envp_ms(t_minishell	*ms)
{
	int	i;

	if (ms && ms->env_is_copy && ms->envp)
	{
		i = 0;
		while (ms->envp[i])
		{
			free(ms->envp[i]);
			ms->envp[i] = NULL;
			i++;
		}
		free(ms->envp);
		ms->envp = NULL;
	}
}

static int	init_args(t_line *cur, char ***args, int *ac, t_minishell *ms)
{
	*ac = 0;
	*args = malloc(sizeof(char *) * (count_args(cur) + 1));
	if (!(*args))
	{
		check_and_free(ms, cur, 1);
		exit(1);
	}
	ll_to_args(*args, ac, cur, ms);
	if (*ac == 0)
	{
		check_and_free(ms, cur, 1);
		exit(1);
	}
	return (0);
}

static char	*exec_args(char **args, t_minishell *ms, t_line *cur)
{
	char	*exec_path;

	exec_path = get_exec_path(args, ms);
	if (!exec_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_args(args, ms->exec_ac);
		ms->exec_args = NULL;
		check_and_free(ms, cur, 1);
		g_code(127);
		exit(127);
	}
	return (exec_path);
}

static void	handle_exec_path(t_line *cur, t_minishell *ms)
{
	if (ft_strchr(cur->value, '/'))
	{
		ft_puterror(cur, ms, NULL);
		if (is_directory(cur->value))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cur->value, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			check_and_free(ms, cur, 0);
			g_code(126);
			exit(126);
		}
		ms->exec_path = ft_strdup(cur->value, ms);
		init_args(cur, &ms->exec_args, &ms->exec_ac, ms);
	}
	else
	{
		init_args(cur, &ms->exec_args, &ms->exec_ac, ms);
		ms->exec_path = exec_args(ms->exec_args, ms, cur);
		ft_puterror(cur, ms, ms->exec_path);
	}
}

void	child_exec(t_line *cur, t_minishell *ms, int heredoc_fd)
{
	if (cur->value[0] == '\0')
	{
		check_and_free(ms, cur, 0);
		exit(0);
	}
	handle_exec_path(cur, ms);
	apply_redirections(cur, heredoc_fd);
	execve(ms->exec_path, ms->exec_args, ms->envp);
	perror("execve error");
	check_and_free(ms, cur, 1);
	exit(EXIT_FAILURE);
}
