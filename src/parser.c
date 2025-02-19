/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:32:13 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/17 09:03:03 by fkuyumcu         ###   ########.fr       */
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
        write(1, "error\n", 6);//erroru düzelt
}


void proc_env(char **input, token_t tokens[], int *count)
{
    //TODO
}

char *ft_strndup(const char *src, size_t n) {
    char *dest = (char *)malloc(n + 1);
    if (!dest) return NULL;
    strncpy(dest, src, n);//strncpy
    dest[n] = '\0';
    return dest;
}

void double_quote(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    quote_checker(minishell, *input);
    (*input)++;

    char *start = *input;

    while (**input && **input != '\"')
        (*input)++;

    if(**input == '\"')
    {
        int len;

        len = *input - start;

        tokens[*count].t_type = TOKEN_WORD;
        tokens[*count].value = strndup(start, len);
        (*count)++;
        (*input)++;
    }
}

int is_delimiter(char c) {
    return c == '\0' || isspace(c) || c == '|' || c == '<' || c == '>' || c == '$';
}

void single_quote(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    quote_checker(minishell, *input);
    (*input)++;

    char *start = *input;

    while (**input && **input != '\"')
        (*input)++;

    if(**input == '\"')
    {
        int len;

        len = *input - start;

        tokens[*count].t_type = TOKEN_WORD;
        tokens[*count].value = strndup(start, len);
        (*count)++;
        (*input)++;
    }
}

void process_word(char **input, token_t tokens[], int *count) {
    char *start = *input;
    
    while (!is_delimiter(**input)) {
        (*input)++;
    }

    int length = *input - start;
    if (length == 0)
        return;

    tokens[*count].t_type = TOKEN_WORD;
    tokens[*count].value = strndup(start, length);//strndup
    (*count)++;
}

void process_token(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
   
    if (**input == '|')
        tokens[(*count)++] = (token_t){TOKEN_PIPE, strdup("|")};
    else if (**input == '<' && *(*input + 1) == '<')
        tokens[(*count)++] = (token_t){TOKEN_REDIRECT_APPEND_IN, strdup("<<")}; 
    else if (**input == '>' && *(*input + 1) == '>')
        tokens[(*count)++] = (token_t){TOKEN_REDIRECT_APPEND_OUT, strdup(">>")};
    else if (**input == '<')
        tokens[(*count)++] = (token_t){TOKEN_REDIRECT_IN,  "<"};
    else if (**input == '>')
        tokens[(*count)++] = (token_t){TOKEN_REDIRECT_OUT, ">"};
    else if (**input == '\"')
        double_quote(input, tokens, count, minishell);
    else if (**input == '\'')
        single_quote(input, tokens, count, minishell);
    else if (**input == '$')
        proc_env(input, tokens, count);
    else 
        process_word(input, tokens, count);
}

void lex_analize(char *input, token_t *tokens, minishell_t *minishell)
{
    int count = 0;

    while (*input != '\0')
    {
        while (*input == ' ')
            input++;
        
        if (*input == '\0')
            break;
        process_token(&input, tokens, &count, minishell);
        ;
        if (strcmp(tokens[count - 1].value, "<<") == 0 || strcmp(tokens[count - 1].value, ">>") == 0)//strcmp->ft_strcmp
            input++;
        input++;
    }
    tokens[count] =  (token_t){TOKEN_END, ""};
}

void parser(char *buf)
{
    minishell_t *minishell;

    minishell = malloc(sizeof(minishell_t));
    
    token_t *tokens = malloc(sizeof(token_t) * MAX_TOKEN_LEN);//memory efficient değil
    lex_analize(buf, tokens, minishell);
     printf("%s\n", tokens[0].value);
     /*
    printf("%s\n", tokens[1].value);
    printf("%s\n", tokens[2].value);
    printf("%s\n", tokens[3].value);
    printf("%s\n", tokens[4].value);
    printf("%s\n", tokens[5].value);
    printf("%s\n", tokens[6].value);  */
}