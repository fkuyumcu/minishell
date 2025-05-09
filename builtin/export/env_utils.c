/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:00:00 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/08 12:00:00 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../src/minishell.h"

void	free_env_array(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		if (env[i])
			free(env[i]);
		i++;
	}
	free(env);
}

void	replace_env_var(t_minishell *ms, int idx, char *arg)
{
	if (ms->env_is_copy)
		free(ms->envp[idx]);
	ms->envp[idx] = ft_strdup(arg, ms);
}

void	add_env_var(t_minishell *ms, char *arg, int size)
{
	char	**old_env;
	char	**new_env;

	old_env = ms->envp;
	new_env = alloc_new_env(size + 1);
	if (!new_env)
		return ;
	copy_env_vars(new_env, ms->envp, size, ms);
	new_env[size] = ft_strdup(arg, ms);
	ms->envp = new_env;
	if (ms->env_is_copy)
		free_env_array(old_env);
	ms->env_is_copy = 1;
}

int	is_valid_identifier(t_line *ml)
{
	int	i;

	if (ml->is_dbl_quote || ml->is_single_quote)
	{
		return (1);
	}
	if (!ml->value || !ml->value[0] || (!ft_isalpha(ml->value[0])
			&& ml->value[0] != '_'))
		return (0);
	i = 1;
	while (ml->value[i] && ml->value[i] != '=')
	{
		if (!ft_isalnum(ml->value[i]) && ml->value[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
