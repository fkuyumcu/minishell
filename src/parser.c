/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:32:13 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/15 16:55:14 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void quote_checker(minishell_t *a, char *str)
{
    int i = 0;
    a->double_quote_flag = 0;
    a->single_quote_flag = 0;
    while (str[i])
    {
        if (str[i] == '"' || str[i] == '\'')
        {
            if (str[i] == '"' && a->single_quote_flag % 2 == 0)
            {
                a->double_quote_flag++;
            }
            else if (str[i] == '\'' && a->double_quote_flag % 2 == 0)
            {
                a->single_quote_flag++;
            }
        }
        i++;
    }
    if (a->double_quote_flag % 2 != 0 || a->single_quote_flag % 2 != 0)
        write(1, "error\n", 6);
}

void redirect_append_out(char **input, token_t tokens[], int *count)
{
    //TODO
}


void redirect_append_in(char **input, token_t tokens[], int *count)
{
    //TODO    
}

void proc_env(char **input, token_t tokens[], int *count)
{
    //TODO
}

void double_quote(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    quote_checker(minishell, *input);
}


void single_quote(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    quote_checker(minishell, *input);
    
}

void process_word(char **input, token_t tokens[], int *count)
{
    char *start = *input;
    while (**input != '\0' && !isspace(**input) && **input != '|' &&
           **input != '<' && **input != '>' && **input != '&' && **input != '\'' && **input != '"')
        (*input)++;

    int length = *input - start;
    if (length >= MAX_TOKEN_LEN) 
        length = MAX_TOKEN_LEN - 1;

    ft_strncpy(tokens[*count].value, start, length);
    tokens[*count].value[length] = '\0';
    tokens[*count].t_type = TOKEN_WORD;
    (*count)++;
}

void process_token(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
   
    if (**input == '|')
        tokens[(*count)++] = (token_t){TOKEN_PIPE, '|'};
    else if (**input == '<' && *(*input + 1) == '<')
        tokens[(*count)++] = (token_t){TOKEN_REDIRECT_APPEND_IN, 'x'}; 
    else if (**input == '>' && *(*input + 1) == '>')
        tokens[(*count)++] = (token_t){TOKEN_REDIRECT_APPEND_OUT, 'y'};
    else if (**input == '<')
        tokens[(*count)++] = (token_t){TOKEN_REDIRECT_IN, '<'};
    else if (**input == '>')
        tokens[(*count)++] = (token_t){TOKEN_REDIRECT_OUT, '>'};
    else if (**input == '&')
        tokens[(*count)++] = (token_t){TOKEN_AMPERSAND, '&'};
    else if (**input == '\"')
        double_quote(input, tokens, count, minishell);
    else if (**input == '\'')
        single_quote(input, tokens, count, minishell);
    else if (**input == '$')
        proc_env(input, tokens, count);
    else 
        process_word(input, tokens, count);
}

void lex_analize(char *input, token_t tokens[], minishell_t *minishell)
{
    int count = 0;

    while (*input != '\0')
    {
        while (*input == ' ')
            input++;
        
        if (*input == '\0')
            break;
        process_token(&input, tokens, &count, minishell);
        if (tokens[count - 1].value[0] == 'x' || tokens[count - 1].value[0] == 'y')
            input++;
        input++;
    }
    tokens[count] =  (token_t){TOKEN_END, ""};
}

void parser(char *buf)
{
    minishell_t *minishell;

    minishell = malloc(sizeof(minishell_t));
    
    token_t tokens[MAX_TOKEN_LEN];
    lex_analize(buf, tokens, minishell);
    printf("%d", tokens[1].t_type);
}