/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08  by fkuyumcu                #+#    #+#             */
/*   Updated: 2025/05/08  by fkuyumcu               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../src/minishell.h"

void	free_envp(char **envp)
{
	if (envp)
		free(envp);
}

void	set_new_var(t_minishell *ms, char *new_var, int size)
{
	char	**new_envp;

	new_envp = create_new_envp(ms->envp, size);
	if (!new_envp)
	{
		free(new_var);
		return ;
	}
	new_envp[size] = new_var;
	if (ms->env_is_copy)
		free_envp(ms->envp);
	ms->envp = new_envp;
	ms->env_is_copy = 1;
}

char	*create_env_string(t_minishell *ms, char *name, char *value)
{
	char	*new_var;
	char	*temp;

	if (!name || !value)
		return (NULL);
	temp = ft_strjoin((char *)name, "=", ms);
	if (!temp)
		return (NULL);
	new_var = ft_strjoin(temp, (char *)value, ms);
	free(temp);
	return (new_var);
}

void	update_env_var(t_minishell *ms, char *name, char *value)
{
	int		index;
	int		size;
	char	*new_var;

	new_var = create_env_string(ms, name, value);
	if (!new_var)
		return ;
	index = cd_find_env_index(ms->envp, name);
	size = 0;
	while (ms->envp && ms->envp[size])
		size++;
	if (index >= 0)
	{
		if (ms->env_is_copy)
		{
			free(ms->envp[index]);
			ms->envp[index] = new_var;
		}
		else
			free(new_var);
	}
	else
		set_new_var(ms, new_var, size);
}
