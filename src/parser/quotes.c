/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:43:16 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/06 16:35:28 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	dnwhile(char quo, char *buffer, char **input, int *buf_index)
{
	while (**input && **input != quo)
	{
		buffer[(*buf_index)++] = **input;
		(*input)++;
	}
}

void	double_quote(char **input, token_t tk[], int *count, minishell_t *ms)
{
	char	buffer[ms->allocation];
	int		buf_index;

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
		printf("Error: Unclosed double quote\n");//error
		return ;
	}
	buffer[buf_index] = '\0';
	tk[*count].is_dbl_quote = 1;
	tk[*count].t_type = WORD;
	tk[*count].value = strdup(buffer);//strdup
	(*count)++;
	process_token(input, tk, count, ms);
}

void	single_quote(char **input, token_t tk[], int *count, minishell_t *ms)
{
	char	buffer[ms->allocation];
	int		buf_index;

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
		printf("Error: Unclosed single quote\n");//error
		return ;
	}
	buffer[buf_index] = '\0';
	tk[*count].is_dbl_quote = 0;
	tk[*count].t_type = WORD;
	tk[*count].value = strdup(buffer);//strdup
	(*count)++;
	process_token(input, tk, count, ms);
}
