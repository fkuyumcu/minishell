/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:43:16 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/22 14:14:37 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void double_quote(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    char buffer[minishell->allocation];
    int buf_index = 0;

    (*input)++; 

    while (**input && **input != '"') {
        buffer[buf_index++] = **input;
        (*input)++;
    }

    if (**input == '"') {
        (*input)++;
    } else {
        fprintf(stderr, "Error: Unclosed single quote\n");//error
        return;
    }

    buffer[buf_index] = '\0';

    tokens[*count].t_type = TOKEN_WORD;
    tokens[*count].value = strdup(buffer);//strdup
    (*count)++;
    process_token(input, tokens, count, minishell);
}


void single_quote(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    char buffer[minishell->allocation];
    int buf_index = 0;

    (*input)++; 

    while (**input && **input != '\'') {
        buffer[buf_index++] = **input;
        (*input)++;
    }

    if (**input == '\'') {
        (*input)++;
    } else {
        fprintf(stderr, "Error: Unclosed single quote\n");//error
        return;
    }

    buffer[buf_index] = '\0';

    tokens[*count].t_type = TOKEN_WORD;
    tokens[*count].value = strdup(buffer);//strdup
    (*count)++;
    process_token(input, tokens, count, minishell);
}