/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swp_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 02:30:57 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/09 09:01:12 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../src/minishell.h"

static void	swap_env_vars(char **env, int a, int b)
{
	char	*temp;

	temp = env[a];
	env[a] = env[b];
	env[b] = temp;
}

static void	sort_env_vars(char **sorted_env, int size)
{
	int	i;
	int	j;
	int	swapped;

	i = 0;
	while (i < size - 1)
	{
		swapped = 0;
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strncmp(sorted_env[j], sorted_env[j + 1],
					ft_strlen(sorted_env[j])) > 0)
			{
				swap_env_vars(sorted_env, j, j + 1);
				swapped = 1;
			}
			j++;
		}
		if (!swapped)
			break ;
		i++;
	}
}

void	handle_quoted_export(t_line **current, t_minishell *ms)
{
	char	*key;
	char	*val;
	char	*newvar;
	int		idx;
	int		size;

	key = get_env((*current)->value);
	val = (*current)->next->value;
	newvar = make_quoted_var(key, val, (*current)->next->is_dbl_quote);
	if (!newvar)
		return (free(key));
	idx = find_env_index(ms, key);
	size = 0;
	while (ms->envp[size])
		size++;
	if (idx >= 0)
		replace_env_var(ms, idx, newvar);
	else
		add_env_var(ms, newvar, size);
	free(newvar);
	free(key);
	*current = (*current)->next;
}

void	display_sorted_env(t_minishell *ms)
{
	int		i;
	int		size;
	char	**sorted_env;

	i = 0;
	size = 0;
	while (ms->envp[size])
		size++;
	sorted_env = alloc_new_env(size);
	if (!sorted_env)
		return ;
	copy_env_vars(sorted_env, ms->envp, size, ms);
	sort_env_vars(sorted_env, size);
	i = 0;
	while (sorted_env[i])
	{
		printf("declare -x %s\n", sorted_env[i]);
		i++;
	}
	free_env_array(sorted_env);
}
