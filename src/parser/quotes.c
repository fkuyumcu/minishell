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

    while (**input == '"') {  
        (*input)++;

        while (**input && **input != '"') {
            buffer[buf_index++] = **input;
            (*input)++;
        }

        if (**input == '"') {
            (*input)++;
        } else {
            fprintf(stderr, "Error: Unclosed double quote\n");
            return;
        }
    }

    while (**input && **input != ' ' && **input != '"' && **input != '\'') {
        buffer[buf_index++] = **input;
        (*input)++;
    }

    buffer[buf_index] = '\0';

    tokens[*count].t_type = TOKEN_WORD;
    tokens[*count].value = strdup(buffer);
    tokens[*count].is_dbl_quote = 1;
    (*count)++;

    if (**input == '\'') {
        single_quote(input, tokens, count, minishell);
    } else {
        process_token(input, tokens, count, minishell);
    }
}


void single_quote(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    char buffer[minishell->allocation];
    int buf_index = 0;

    while (**input == '\'') {  
        (*input)++;

        while (**input && **input != '\'') {
            buffer[buf_index++] = **input;
            (*input)++;
        }

        if (**input == '\'') {
            (*input)++;
        } else {
            fprintf(stderr, "Error: Unclosed double quote\n");
            return;
        }
    }

    while (**input && **input != ' ' && **input != '"' && **input != '\'') {
        buffer[buf_index++] = **input;
        (*input)++;
    }

    buffer[buf_index] = '\0';

    tokens[*count].t_type = TOKEN_WORD;
    tokens[*count].value = strdup(buffer);
    tokens[*count].is_dbl_quote = 1;
    (*count)++;

    if (**input == '"') {
        double_quote(input, tokens, count, minishell);
    } else {
        process_token(input, tokens, count, minishell);
    }
}