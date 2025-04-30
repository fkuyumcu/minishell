/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:16:06 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/30 15:49:29 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"
#include <stdio.h>



void	env(minishell_t *ms)
{
    int	i;

    i = 0;
    while (ms->envp[i])
    {
        printf("%s\n", ms->envp[i]);
        i++;
    }
}

void ft_exit_2()
{
    
}