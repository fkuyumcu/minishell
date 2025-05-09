/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:05:44 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/09 08:36:17 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

// static yardımcı fonksiyonlar ve free_filtered utils_etc.c'ye taşındı

t_line	*filter_args(t_line *ml, t_minishell *ms)
{
	t_line	*new_head;
	t_line	*current;
	t_line	*temp;
	t_line	*prev;

	if (!ml)
		return (NULL);
	temp = copy_line(ml, ms);
	if (!temp)
		return (NULL);
	new_head = temp;
	current = temp;
	prev = ml;
	ml = ml->next;
	filter_args_loop(&ml, &prev, &current, ms);
	return (new_head);
}

bool	is_builtin(t_line *ml)
{
	char	*cmd;

	cmd = ml->value;
	return (ft_strncmp(cmd, "echo", 6) == 0 || ft_strncmp(cmd, "cd", 6) == 0
		|| ft_strncmp(cmd, "pwd", 6) == 0 || ft_strncmp(cmd, "export", 6) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0 || ft_strncmp(cmd, "env", 6) == 0
		|| ft_strncmp(cmd, "exit", 6) == 0);
}

void	try_builtins(t_line *ml, t_minishell *ms)
{
	char	*cmd;
	t_line	*filtered;

	filtered = filter_args(ml, ms);
	cmd = filtered->value;
	if (!ft_strncmp(cmd, "echo", 6))
		echo(filtered);
	else if (!ft_strncmp(cmd, "cd", 6))
		cd(filtered, ms);
	else if (!ft_strncmp(cmd, "pwd", 6))
		pwd();
	else if (!ft_strncmp(cmd, "export", 6))
		export(filtered, ms);
	else if (!ft_strncmp(cmd, "unset", 6))
		unset(ml, ms);
	else if (!ft_strncmp(cmd, "env", 6))
		env(ms);
	else if (!ft_strncmp(cmd, "exit", 6))
		bi_exit(filtered, ms);
	free_filtered(filtered);
}
