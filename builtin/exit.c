/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 02:27:24 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/09 08:37:17 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

int	ft_isdigit_str(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

static int	ft_atoi(char *str)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 1;
	while (str[i] == 32 || (str[i] <= 13 && str[i] >= 9))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			k = -1;
		i++;
	}
	while ((str[i] <= 57 && str[i] >= 48))
	{
		j = j * 10 + (str[i] - 48);
		i++;
	}
	return (k * j);
}

int	get_exit_status(t_line *ml)
{
	int	status;

	if (ml && ml->next && ml->next->value
		&& ft_isdigit_str(ml->next->value) != 0)
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		status = 2;
	}
	else if (ml && ml->next && ml->next->next)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		status = 1;
	}
	else if (ml && ml->next && ml->next->value)
	{
		status = ft_atoi(ml->next->value);
		if (status < 0)
			status = 256 + status;
		else if (status > 255)
			status = status % 256;
	}
	else
		status = 0;
	return (status);
}

void	bi_exit(t_line *ml, t_minishell *ms)
{
	int	status;

	write(1, "exit\n", 5);
	status = get_exit_status(ml);
	g_code = status;
	check_and_free(ms, ml, 0);
	exit(g_code);
}
