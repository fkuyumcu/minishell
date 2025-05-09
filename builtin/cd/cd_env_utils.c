/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08  by fkuyumcu                #+#    #+#             */
/*   Updated: 2025/05/08  by fkuyumcu               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../src/minishell.h"

char	*get_env_value(char **env, char *name)
{
	int		i;
	int		name_len;

	if (!env || !name)
		return (NULL);
	i = 0;
	name_len = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
			return (env[i] + name_len + 1);
		i++;
	}
	return (NULL);
}

int	cd_find_env_index(char **env, char *key)
{
	int		i;
	int		key_len;

	if (!env || !key)
		return (-1);
	i = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0
			&& (env[i][key_len] == '=' || env[i][key_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

char	**create_new_envp(char **old_envp, int size)
{
	char	**new_envp;
	int		i;

	new_envp = malloc(sizeof(char *) * (size + 2));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < size && old_envp[i])
	{
		new_envp[i] = old_envp[i];
		i++;
	}
	new_envp[size] = NULL;
	new_envp[size + 1] = NULL;
	return (new_envp);
}

void	free_envp(char **envp);
void	set_new_var(t_minishell *ms, char *new_var, int size);
char	*create_env_string(t_minishell *ms, char *name, char *value);
void	update_env_var(t_minishell *ms, char *name, char *value);