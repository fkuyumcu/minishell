/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minidup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:58:18 by yalp              #+#    #+#             */
/*   Updated: 2025/05/09 08:45:24 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*minidup(char *str)
{
	int		i;
	char	*ret;

	i = 0;
	while (str[i])
		i++;
	ret = NULL;
	ret = malloc(sizeof(char) * (i + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (str[i])
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

void	dnwhile(char quo, char *buffer, char **input, int *buf_index)
{
	while (**input && **input != quo)
	{
		buffer[(*buf_index)++] = **input;
		(*input)++;
	}
}

void	quote_supply(char *buffer, int buf_index, int *count, t_token *tk)
{
	buffer[buf_index] = '\0';
	tk[*count].t_type = WORD;
	tk[*count].value = minidup(buffer);
	if (tk[*count].is_dbl_quote != 1 && tk[*count].is_single_quote != 1)
	{
		tk[*count].is_dbl_quote = 0;
		tk[*count].is_single_quote = 0;
	}
	(*count)++;
}
