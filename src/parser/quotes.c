/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:43:16 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/20 18:38:29 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void double_quote(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    char buffer[minishell->allocation];
    int buf_index = 0;

    (*input)++; // Skip the opening single quote

    while (**input && **input != '"') {
        buffer[buf_index++] = **input;
        (*input)++;
    }

    if (**input == '"') {
        (*input)++; // Skip the closing single quote
    } else {
        fprintf(stderr, "Error: Unclosed single quote\n");//error
        return;
    }

    buffer[buf_index] = '\0';

    tokens[*count].t_type = TOKEN_WORD;
    tokens[*count].value = strdup(buffer);//strdup
    (*count)++;
}


void single_quote(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    char buffer[minishell->allocation];
    int buf_index = 0;

    (*input)++; // Skip the opening single quote

    while (**input && **input != '\'') {
        buffer[buf_index++] = **input;
        (*input)++;
    }

    if (**input == '\'') {
        (*input)++; // Skip the closing single quote
    } else {
        fprintf(stderr, "Error: Unclosed single quote\n");//error
        return;
    }

    buffer[buf_index] = '\0';

    tokens[*count].t_type = TOKEN_WORD;
    tokens[*count].value = strdup(buffer);//strdup
    (*count)++;
}