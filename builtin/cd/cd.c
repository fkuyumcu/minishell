/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 03:46:51 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/08  by fkuyumcu               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../src/minishell.h"

int	cd_check_and_prepare(t_line *ml, char **old_pwd, t_minishell *ms)
{
	*old_pwd = get_current_directory(ms);
	if (!*old_pwd)
		return (1);
	if (ml->next && ml->next->next != NULL
		&& is_directory(ml->next->value) != 1)
	{
		ft_putstr_fd(" No such file or directory", 2);
		g_code(1);
		free(*old_pwd);
		return (1);
	}
	if (ml->next && ml->next->next != NULL)
	{
		ft_putstr_fd(" too many arguments", 2);
		g_code(1);
		free(*old_pwd);
		return (1);
	}
	return (0);
}

void	cd(t_line *ml, t_minishell *ms)
{
	char	*old_pwd;
	char	*new_pwd;

	if (cd_check_and_prepare(ml, &old_pwd, ms))
		return ;
	if (handle_cd_args(ml, ms, old_pwd))
		return ;
	new_pwd = get_current_directory(ms);
	if (!new_pwd)
	{
		free(old_pwd);
		return ;
	}
	update_pwd_and_oldpwd(ms, old_pwd, new_pwd);
	g_code(0);
}
