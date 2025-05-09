/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_actions_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08  by fkuyumcu                #+#    #+#             */
/*   Updated: 2025/05/08  by fkuyumcu               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../src/minishell.h"

int	cd_to_home(t_minishell *ms)
{
	char	*home_dir;

	home_dir = get_env_value(ms->envp, "HOME");
	if (!home_dir)
	{
		ft_putstr_fd("minishell: cd: HOME değişkeni tanımlı değil\n", 2);
		g_code = 1;
		return (1);
	}
	if (chdir(home_dir) != 0)
	{
		perror("minishell: cd");
		g_code = 1;
		return (1);
	}
	return (0);
}

int	cd_to_previous(t_minishell *ms)
{
	char	*oldpwd;

	oldpwd = get_env_value(ms->envp, "OLDPWD");
	if (!oldpwd)
	{
		ft_putstr_fd("minishell: cd: OLDPWD değişkeni tanımlı değil\n", 2);
		g_code = 1;
		return (1);
	}
	if (chdir(oldpwd) != 0)
	{
		perror("minishell: cd");
		g_code = 1;
		return (1);
	}
	printf("%s\n", oldpwd);
	return (0);
}

int	cd_to_path(char *path, char *old_pwd)
{
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		g_code = 1;
		free(old_pwd);
		return (1);
	}
	return (0);
}

int	handle_cd_args(t_line *ml, t_minishell *ms, char *old_pwd)
{
	if (!ml->next)
		return (cd_to_home(ms));
	if (ml->next->next != NULL)
	{
		ft_putstr_fd("minishell: cd: çok fazla argüman\n", 2);
		g_code = 1;
		free(old_pwd);
		return (1);
	}
	if (ft_strncmp(ml->next->value, "-", 2) == 0)
		return (cd_to_previous(ms));
	if (ft_strncmp(ml->next->value, "~", 2) == 0)
		return (cd_to_home(ms));
	return (cd_to_path(ml->next->value, old_pwd));
}
