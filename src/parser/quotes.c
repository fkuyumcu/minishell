/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:43:16 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/19 13:49:44 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void double_quote(char **input, token_t tokens[], int *count)
{
    (*input)++;

    char *start = *input;

    while (**input && **input != '\"')
        (*input)++;

    if(**input == '\"')
    {
        int len;

        len = *input - start;

        tokens[*count].t_type = TOKEN_WORD;
        tokens[*count].value = ft_strndup(start, len);
        (*count)++;
        (*input)++;
    }
    else
        printf("error");//error
}



void single_quote(char **input, token_t tokens[], int *count)
{
    
    (*input)++;

    char *start = *input;

    while (**input && **input != '\"')
        (*input)++;

    if(**input == '\"')
    {
        int len;

        len = *input - start;

        tokens[*count].t_type = TOKEN_WORD;
        tokens[*count].value = ft_strndup(start, len);
        (*count)++;
        (*input)++;
    }
    else
        printf("error");//error
}