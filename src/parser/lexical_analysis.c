/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analysis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:45:25 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/19 14:07:51 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void proc_env(char **input, token_t tokens[], int *count)
{
    //TODO
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
    tokens[*count].value = ft_strndup(start, length);
    (*count)++;
}

void process_token(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
   
    if (**input == '|')
        tokens[(*count)++] = (token_t){TOKEN_PIPE, ft_strndup("|", 1)};
    else if (**input == '<' && *(*input + 1) == '<')
        tokens[(*count)++] = (token_t){TOKEN_REDIRECT_APPEND_IN, ft_strndup("<<", 2)}; 
    else if (**input == '>' && *(*input + 1) == '>')
        tokens[(*count)++] = (token_t){TOKEN_REDIRECT_APPEND_OUT, ft_strndup(">>", 2)};
    else if (**input == '<')
        tokens[(*count)++] = (token_t){TOKEN_REDIRECT_IN,  ft_strndup("<", 1)};
    else if (**input == '>')
        tokens[(*count)++] = (token_t){TOKEN_REDIRECT_OUT, ft_strndup(">", 1)};
    else if (**input == '\"')
        double_quote(input, tokens, count);
    else if (**input == '\'')
        single_quote(input, tokens, count);
    else if (**input == '$')
        proc_env(input, tokens, count);
    else 
        process_word(input, tokens, count);
    //token'ları freeleme fonksiyonu yap
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
        if (ft_strncmp(tokens[count - 1].value, "<<", 3) == 0 || ft_strncmp(tokens[count - 1].value, ">>", 3) == 0)
            input++;
        input++;
    }
    tokens[count] =  (token_t){TOKEN_END, ""};
}


