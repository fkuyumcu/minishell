/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:30:44 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/03 12:44:26 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_str_array(char **paths, int i)
{
	int	j;

	j = 0;
	if (paths)
	{
		while (j < i)
		{
			if (paths[j])
				free(paths[j]);
			j++;
		}
		while (paths[j])
		{
			free(paths[j]);
			j++;
		}
	}
}

void	free_args(char **args, int ac)
{
	int	i;

	i = 0;
	while (i < ac)
		free(args[i++]);
	free(args);
}
