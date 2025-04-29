/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:43:16 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/29 18:10:16 by yalp             ###   ########.fr       */
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

void	quote_supply(char *buffer, int buf_index, int *count, token_t *tk)
{
	buffer[buf_index] = '\0';
	tk[*count].t_type = WORD;
	tk[*count].value = minidup(buffer);
	(*count)++;
}

void	double_quote(char **input, token_t tk[], int *count, minishell_t *ms)
{
	char	*buffer;
	int		buf_index;

	buffer = malloc(sizeof(char) * ms->allocation);
	if (!buffer)
		return ;
	buf_index = 0;
	if (*(*input - 1) == ' ')
		tk[*count].space_flag = 1;
	else
		tk[*count].space_flag = 0;
	(*input)++;
	while (**input && **input != '"')
		dnwhile('\"', buffer, input, &buf_index);
	if (**input == '"')
		(*input)++;
	else
	{
		printf("Error: Unclosed double quote\n");
		ms->error = 1;
		return (free(buffer));
	}
	tk[*count].is_dbl_quote = 1;
	quote_supply(buffer, buf_index, count, tk);
	free(buffer);
	process_token(input, tk, count, ms);
}

void	single_quote(char **input, token_t tk[], int *count, minishell_t *ms)
{
	char	*buffer;
	int		buf_index;

	buffer = malloc(sizeof(char) * ms->allocation);
	if (!buffer)
		return ;
	buf_index = 0;
	if (*(*input - 1) == ' ')
		tk[*count].space_flag = 1;
	else
		tk[*count].space_flag = 0;
	(*input)++;
	while (**input && **input != '\'')
		dnwhile('\'', buffer, input, &buf_index);
	if (**input == '\'')
		(*input)++;
	else
	{
		printf("Error: Unclosed single quote\n");
		return (free(buffer));
	}
	tk[*count].is_dbl_quote = 0;
	quote_supply(buffer, buf_index, count, tk);
	free(buffer);
	process_token(input, tk, count, ms);
}
