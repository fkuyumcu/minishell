/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:43:16 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/20 13:30:04 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void double_quote(char **input, token_t tokens[], int *count)
{
    char buffer[1024];  // Birleştirilecek string için geçici buffer
    int buf_index = 0;
    char *deneme = *input;

    while (**input == '\"') {  // Sürekli çift tırnak yakalayana kadar döngüye gir
        (*input)++;  // Açılış çift tırnağını geç

        while (**input && **input != '\"') {
            buffer[buf_index++] = **input;
            (*input)++;
        }

        if (**input == '\"')
            (*input)++;  // Kapanış çift tırnağını atla
        else {
            fprintf(stderr, "Error: Unclosed double quote\n");
            return;
        }
    }
    if (**input != ' ' && **input != '\0')
    {
        while (**input != ' ' && **input != '\0')
        {
            buffer[buf_index++] = **input;
            (*input)++;
        }
    }
    buffer[buf_index] = '\0';  // String'in sonunu belirle

    tokens[*count].t_type = TOKEN_WORD;
    tokens[*count].value = strdup(buffer);  // String'i kopyala ve token'a ata
    (*count)++;
}



void single_quote(char **input, token_t tokens[], int *count)
{
    
    char buffer[1024];  // Birleştirilecek string için geçici buffer
    int buf_index = 0;
    char *deneme = *input;

    while (**input == '\'') {  // Sürekli çift tırnak yakalayana kadar döngüye gir
        (*input)++;  // Açılış çift tırnağını geç

        while (**input && **input != '\'') {
            buffer[buf_index++] = **input;
            (*input)++;
        }

        if (**input == '\'')
            (*input)++;  // Kapanış çift tırnağını atla
        else {
            fprintf(stderr, "Error: Unclosed double quote\n");
            return;
        }
    }
    if (**input != ' ' && **input != '\0')
    {
        while (**input != ' ' && **input != '\0')
        {
            buffer[buf_index++] = **input;
            (*input)++;
        }
    }
    buffer[buf_index] = '\0';  // String'in sonunu belirle

    tokens[*count].t_type = TOKEN_WORD;
    tokens[*count].value = strdup(buffer);  // String'i kopyala ve token'a ata
    (*count)++;
}