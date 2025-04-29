/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:30:44 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/29 12:50:07 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_str_array(char **paths, int i)
{
	if (paths[i])
	{
		while (paths[++i])
			free(paths[i]);
	}

}


void	free_args(char **args, int ac)
{
    int	i = 0;
    while (i < ac)
        free(args[i++]);
    free(args);
}