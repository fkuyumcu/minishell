/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 03:48:21 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/03 12:42:28 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

static int	unset_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0] || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	unset_find_env_index(t_minishell *ms, char *key)
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

static void	remove_env(t_minishell *ms, int index)
{
	int	i;
	int	count;

	if (index < 0)
		return ;
	count = 0;
	while (ms->envp[count])
		count++;
	if (ms->env_is_copy && ms->envp[index])
	{
		free(ms->envp[index]);
		ms->envp[index] = NULL;
	}
	i = index;
	while (i < count)
	{
		ms->envp[i] = ms->envp[i + 1];
		i++;
	}
}

void	unset(t_line *ml, t_minishell *ms)
{
	t_line	*current;
	int		index;

	current = ml->next;
	while (current)
	{
		if (!unset_valid_identifier(current->value))
		{
			printf("minishell: unset: `%s': not a valid identifier\n",
				current->value);
		}
		else
		{
			index = unset_find_env_index(ms, current->value);
			if (index >= 0)
				remove_env(ms, index);
		}
		current = current->next;
	}
}
