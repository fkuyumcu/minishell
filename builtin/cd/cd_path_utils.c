/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_path_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08  by fkuyumcu                #+#    #+#             */
/*   Updated: 2025/05/08  by fkuyumcu               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../src/minishell.h"

char	*get_current_directory(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		perror("minishell: cd");
	return (cwd);
}

void	update_pwd_and_oldpwd(t_minishell *ms, char *old_pwd, char *new_pwd)
{
	if (ft_strncmp(old_pwd, new_pwd, ft_strlen(old_pwd) + 1) == 0)
	{
		free(old_pwd);
		free(new_pwd);
		return ;
	}
	update_env_var(ms, "OLDPWD", old_pwd);
	update_env_var(ms, "PWD", new_pwd);
	free(old_pwd);
	free(new_pwd);
}
