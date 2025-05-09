/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:31:14 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/03 19:28:24 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**split_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	if (!envp[i])
		return (NULL);
	return (ft_split(envp[i] + 5, ':'));
}

static char	*try_path(char *dir, char *cmd, t_minishell *ms)
{
	char	*part_path;
	char	*full_path;

	part_path = ft_strjoin(dir, "/", ms);
	if (!part_path)
		return (NULL);
	full_path = ft_strjoin(part_path, cmd, ms);
	free(part_path);
	if (!full_path)
		return (NULL);
	if (access(full_path, F_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*find(char *cmd, char **envp, t_minishell *ms)
{
	char	**paths;
	char	*path;
	int		i;

	paths = split_path(envp);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = try_path(paths[i], cmd, ms);
		if (path)
		{
			free_str_array(paths, i);
			free(paths);
			return (path);
		}
		i++;
	}
	free_str_array(paths, i);
	free(paths);
	return (NULL);
}

void	ll_to_args(char **args, int *ac, t_line *node, t_minishell *ms)
{
	while (node)
	{
		if (node->type == REDIRECT_IN || node->type == REDIRECT_OUT
			|| node->type == HEREDOC_IN || node->type == HEREDOC_OUT)
		{
			node = node->next;
			if (node)
				node = node->next;
			continue ;
		}
		if (node->type == WORD)
			args[(*ac)++] = ft_strdup(node->value, ms);
		node = node->next;
	}
	args[*ac] = NULL;
}

char	*get_exec_path(char **args, t_minishell *ms)
{
	char	*exec_path;

	exec_path = NULL;
	if (args[0])
		exec_path = find(args[0], ms->envp, ms);
	return (exec_path);
}
