/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:55:27 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/14 19:11:59 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

void priority(minishell_t *ms)
{   
    int max_priority;
    int i;
    line_t *current;

    i = 0;
    
    while(ms->mini_lines[i])
    {
        max_priority = 0;
        current = ms->mini_lines[i];
        while(current != NULL)
        {
            if (current->value != NULL)
            {
                if (ft_strncmp("<<", current->value, 2) == 0 || 
                    ft_strncmp(">>", current->value, 2) == 0)
                {
                    max_priority = 2;
                    break; 
                }
                else if ((ft_strncmp("<", current->value, 1) == 0 || 
                         ft_strncmp(">", current->value, 1) == 0) &&
                         max_priority < 1)
                {
                    max_priority = 1;
                }
            }
            current = current->next;
        }
        if (ms->mini_lines[i] != NULL)
            ms->mini_lines[i]->priority = max_priority;
        i++;
    }
}
