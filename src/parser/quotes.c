/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:43:16 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/04 14:58:56 by fkuyumcu         ###   ########.fr       */
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
void double_quote(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    char	buffer[minishell->allocation];
    int		buf_index;

	buf_index	= 0;
    if (*(*input - 1) == ' ')
       tokens[*count].space_flag = 1;
    else
        tokens[*count].space_flag = 0;
    (*input)++; 
    while (**input && **input != '"')
        dnwhile('\"', buffer, input, &buf_index);
    if (**input == '"')
        (*input)++;
    else
    {
        printf("Error: Unclosed double quote\n");//error
        return;
    }
    buffer[buf_index] = '\0';
    tokens[*count].is_dbl_quote = 1;
    tokens[*count].t_type = WORD;
    tokens[*count].value = strdup(buffer);//strdup
    (*count)++;
    process_token(input, tokens, count, minishell);
}



void single_quote(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    char	buffer[minishell->allocation];
    int		buf_index;

	buf_index = 0;
    if (*(*input - 1) == ' ')
        tokens[*count].space_flag = 1;
    else
        tokens[*count].space_flag = 0;
    (*input)++; 
    while (**input && **input != '\'')
        dnwhile('\'', buffer, input, &buf_index);
    if (**input == '\'')
        (*input)++;
	else 
	{
        printf("Error: Unclosed single quote\n");//error
        return;
    }
    buffer[buf_index] = '\0';
    tokens[*count].is_dbl_quote = 0;
    tokens[*count].t_type = WORD;
    tokens[*count].value = strdup(buffer);//strdup
    (*count)++;
    process_token(input, tokens, count, minishell);
}
