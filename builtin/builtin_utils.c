/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:05:44 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/30 16:01:18 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

bool is_builtin(line_t *ml)//bool
{
    char *cmd;
    cmd = ml->value;

    return (//bunları dene
        ft_strncmp(cmd, "echo", 6) == 0 ||
        ft_strncmp(cmd, "cd", 6) == 0 ||
        ft_strncmp(cmd, "pwd", 6) == 0 ||
        ft_strncmp(cmd, "export", 6) == 0 ||
        ft_strncmp(cmd, "unset", 6) == 0 ||
        ft_strncmp(cmd, "env", 6) == 0 ||
        ft_strncmp(cmd, "exit", 6) == 0
    );
    
    
}

void try_builtins(line_t *ml, minishell_t *ms)
{
    char *cmd;
    cmd = ml->value;
    if(!ft_strncmp(cmd, "echo", 4))
        echo(ml, ms);
    else if(!ft_strncmp(cmd, "cd", 6))
        cd(ml);
    else if(!ft_strncmp(cmd, "pwd", 6))
        pwd();
    else if(!ft_strncmp(cmd, "export", 6))
        export(ml, ms);
    else if(!ft_strncmp(cmd, "unset", 6))
        unset();
    else if(!ft_strncmp(cmd, "env", 6))
        env(ms);
    else if(!ft_strncmp(cmd, "exit", 6))
        ft_exit_2(ml);
    
}