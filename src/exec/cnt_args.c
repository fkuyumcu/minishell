/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cnt_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:45:20 by yalp              #+#    #+#             */
/*   Updated: 2025/05/07 15:56:33 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_args(t_line *cur)
{
	int		count;
	t_line	*tmp;

	count = 0;
	tmp = cur;
	while (tmp && tmp->value)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

int	is_directory(char *path)
{
	struct stat	statbuf;
	char		*dot;

	if (path[0] == '.')
	{
		dot = ft_strchr(path, '/');
		if (dot)
		{
			if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
				return (1);
		}
	}
	else
	{
		if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
			return (1);
	}
	return (0);
}

void	ft_puterror(t_line *cur, t_minishell *ms, char *str)
{
	char	*tmp;

	tmp = cur->value;
	if (str)
		tmp = str;
	if (access(tmp, F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(tmp, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		check_and_free(ms, cur, 0);
		g_code = 127;
		exit(127);
	}
	else if (access(tmp, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(tmp, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		check_and_free(ms, cur, 0);
		g_code = 126;
		exit(126);
	}
}

void	close_fds(void)
{
	int	i;

	i = 3;
	while (i < 256)
	{
		close(i);
		i++;
	}
}

void	free_tokens_end(t_minishell *ms)
{
	int	i;

	if (!ms->tokens)
		return ;
	i = 0;
	while (ms->tokens[i].t_type != TOKEN_END)
	{
		free(ms->tokens[i].value);
		i++;
	}
	free(ms->tokens);
	ms->tokens = NULL;
}
