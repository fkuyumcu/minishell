/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:10:21 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/05 16:44:20 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

int	is_flag_n(const char *arg)
{
	int	i;

	i = 1;
	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	echo(t_line *ml)
{
	int		print_newline;
	t_line	*cur;
	int		has_printed;

	print_newline = 1;
	has_printed = 0;
	cur = ml->next;
	while (cur && is_flag_n(cur->value))
	{
		print_newline = 0;
		cur = cur->next;
	}
	while (cur)
	{
		if (cur->value && cur->value[0] != '\0')
		{
			if (has_printed)
				printf(" ");
			printf("%s", cur->value);
			has_printed = 1;
		}
		cur = cur->next;
	}
	if (print_newline)
		printf("\n");
}
