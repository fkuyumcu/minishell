/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:00:00 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/08 12:00:00 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../src/minishell.h"

char	*get_env(char *arg)
{
	char	*name;
	int		i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	name = malloc(sizeof(char) * (i + 1));
	if (!name)
		return (NULL);
	i = 0;
	while (arg[i] && arg[i] != '=')
	{
		name[i] = arg[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}

int	find_env_index(t_minishell *ms, char *key)
{
	int	i;
	int	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (ms->envp[i])
	{
		if (ft_strncmp(ms->envp[i], key, key_len) == 0
			&& (ms->envp[i][key_len] == '=' || ms->envp[i][key_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

char	**alloc_new_env(int size)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_env[i] = NULL;
		i++;
	}
	new_env[size] = NULL;
	return (new_env);
}

void	copy_env_vars(char **dest, char **src, int size, t_minishell *ms)
{
	int	i;

	i = 0;
	while (i < size && src[i])
	{
		dest[i] = ft_strdup(src[i], ms);
		i++;
	}
}
