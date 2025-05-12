/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:55:30 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/09 14:23:54 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../src/minishell.h"

static void	process_full_value(char *key, char *value, t_minishell *ms)
{
	int	idx;
	int	size;

	idx = find_env_index(ms, key);
	size = 0;
	while (ms->envp[size])
		size++;
	if (idx >= 0)
		replace_env_var(ms, idx, value);
	else
		add_env_var(ms, value, size);
}

static void	update_envp(char *arg, t_line *current, t_minishell *ms)
{
	char	*key;
	char	*full_value;

	full_value = NULL;
	key = get_env(arg);
	if (!key)
		return ;
	if (current->is_dbl_quote || current->is_single_quote)
		full_value = prepare_quoted_value(arg, key);
	else
		full_value = prepare_unquoted_value(arg, key);
	if (full_value)
	{
		process_full_value(key, full_value, ms);
		free(full_value);
		free(key);
		return ;
	}
	process_full_value(key, arg, ms);
	free(key);
}

void	export_loop(t_line *current, t_minishell *ms)
{
	int	klen;

	while (current)
	{
		if (!is_valid_identifier(current))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(current->value, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			g_code(1);
			return ;
		}
		klen = ft_strlen(current->value);
		if (klen > 0 && current->value[klen - 1] == '=' && current->next
			&& (current->next->is_dbl_quote || current->next->is_single_quote))
			handle_quoted_export(&current, ms);
		else
			update_envp(current->value, current, ms);
		current = current->next;
	}
	return ;
}

void	export(t_line *ml, t_minishell *ms)
{
	t_line	*current;

	current = ml->next;
	if (!current)
	{
		display_sorted_env(ms);
		return ;
	}
	export_loop(current, ms);
}
