/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:32:13 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/14 16:43:26 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void lex_analize(char *input, token_t tokens[])
{
    int count = 0;

    while (*input != '\0')
    {
        while (*input == ' ')
            input++;
        if (*input == '\0')
            break;
        if (*input == '|')
        {
            tokens[count++] = (token_t){TOKEN_PIPE, '|'};
            input++;
        }
        else if(*input == '<')
        {
            tokens[count++] = (token_t){TOKEN_REDIRECT_IN, '<'};
            input++;
        }
        else if(*input == '>')
        {
            tokens[count++] = (token_t){TOKEN_REDIRECT_OUT, '>'};
            input++;
        }
        else if(*input == '&')
        {
            tokens[count++] = (token_t){TOKEN_AMPERSAND, '&'};
            input++;
        }
        else 
        {
            char *start = input;
            while (*input != '\0' && !isspace(*input) && *input != '|' &&
                   *input != '<' && *input != '>' && *input != '&')
                input++;
            int length = input - start;
            if (length >= MAX_TOKEN_LEN) 
                length = MAX_TOKEN_LEN - 1;
            ft_strncpy(tokens[count].value, start, length);
            tokens[count].value[length] = '\0';
            tokens[count].t_type = TOKEN_WORD;
            count++;
        }
    }
    tokens[count] =  (token_t){TOKEN_END, ""};
}

void parser(char *buf)
{
    token_t tokens[MAX_TOKEN_LEN];
    lex_analize(buf, tokens);

    
}