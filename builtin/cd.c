/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:32:51 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/30 15:43:18 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../src/minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static char	*get_cd_path(line_t *ml)
{
    if (!ml || !ml->value || ft_strncmp(ml->value, "~", 2) == 0)//bu kodları gözden geçir
    {
        if (!getenv("HOME"))
        {
            printf("minishell: cd: HOME not set\n");
            return (NULL);
        }
        return (getenv("HOME"));
    }
    else if (ft_strncmp(ml->value, "-", 2) == 0)
    {
        if (!getenv("OLDPWD"))
        {
            printf("minishell: cd: OLDPWD not set\n");
            return (NULL);
        }
        printf("%s\n", getenv("OLDPWD"));
        return (getenv("OLDPWD"));
    }
    return (ml->value);
}

static int	update_env(char *old_pwd)
{
    char	current_dir[500];//sabit bufsize!!

    if (getcwd(current_dir, 500) == NULL)
    {
        printf("minishell: cd: getcwd error\n");
        free(old_pwd);
        return (1);
    }
    setenv("OLDPWD", old_pwd, 1);
    setenv("PWD", current_dir, 1);
    free(old_pwd);
    return (0);
}

void	cd(line_t *ml)
{
    char	*dir_path;
    char	*old_pwd;
    char	current_dir[500];

    ml = ml->next;
    if (getcwd(current_dir, 500) == NULL)
    {
        printf("minishell: cd: getcwd error\n");
        return ;
    }
    dir_path = get_cd_path(ml);
    if (!dir_path)
        return ;
    old_pwd = strdup(current_dir);
    if (!old_pwd)
    {
        printf("minishell: cd: memory error\n");
        return ;
    }
    if (chdir(dir_path) != 0)
    {
        printf("minishell: cd: %s: ", dir_path);
        free(old_pwd);
        return ;
    }
    update_env(old_pwd);
}