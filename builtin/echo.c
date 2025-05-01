/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:10:21 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/01 14:03:25 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

int	is_flag_n(const char *arg)
{
    int i = 1;
    if (!arg || arg[0] != '-' || arg[1] != 'n')
        return 0;
    while (arg[i])
    {
        if (arg[i] != 'n')
            return 0;
        i++;
    }
    return 1;
}


void	echo(line_t *ml, minishell_t *ms)
{
    int print_newline;
    line_t *cur;
    
    print_newline = 1;
    cur = ml->next;
    
    while (cur && is_flag_n(cur->value))
    {
        print_newline = 0;
        cur = cur->next;
    }
    
    while (cur)
    {
            printf("%s", cur->value);
            
        if (cur->next)
            printf(" ");
        cur = cur->next;
    }
    
    if (print_newline)
        printf("\n");
}