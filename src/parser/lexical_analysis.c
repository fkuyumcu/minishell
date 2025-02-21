/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analysis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:45:25 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/22 14:29:11 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void proc_env(token_t *token, minishell_t *minishell)
{
    token->is_env = 1;

}



void check_env(token_t tokens[], minishell_t *minishell)
{
    
}

void process_word(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    char buffer[minishell->allocation];
    int buf_index = 0;

    while (**input && **input != ' ' && **input != '|' && **input != '<' && **input != '>' && **input != '"' && **input != '\'')
    {
        buffer[buf_index++] = **input;
        (*input)++;
    }

    buffer[buf_index] = '\0';

    if (buf_index > 0) 
    {
        tokens[*count].t_type = TOKEN_WORD;
        tokens[*count].value = strdup(buffer);//strdup
        (*count)++;
    }
    if (**input == '|' || **input == '<' || **input == '>' || **input == '"' || **input == '\'')
        process_token(input, tokens, count, minishell);
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
        double_quote(input, tokens, count, minishell);
    else if (**input == '\'')
        single_quote(input, tokens, count, minishell);
    else 
        process_word(input, tokens, count, minishell);
    minishell->count_token++;
}

void lex_analize(char *input, token_t *tokens, minishell_t *minishell)
{
    int count = 0;
    
    if (input == NULL)
        return ;

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
    minishell->count = count;
}