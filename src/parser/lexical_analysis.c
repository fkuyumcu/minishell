/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analysis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:45:25 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/03 13:35:43 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void process_word(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    char buffer[minishell->allocation];
    int buf_index = 0;
    if ( *(*input - 1) == ' ')
       tokens[*count].space_flag = 1;
    else
        tokens[*count].space_flag = 0;
    while (**input && **input != ' ' && **input != '|' && **input != '<' && **input != '>' && **input != '"' && **input != '\'')
    {
        buffer[buf_index++] = **input;
        (*input)++;
    }

    buffer[buf_index] = '\0';

    if (buf_index > 0) 
    {
        tokens[*count].is_word = 1;
        tokens[*count].is_dbl_quote = 1;
        tokens[*count].t_type = WORD;
        tokens[*count].value = ft_strdup(buffer);
        (*count)++;
    }
    if (**input == '|' || **input == '<' || **input == '>' || **input == '"' || **input == '\'')
        process_token(input, tokens, count, minishell);
}

void process_token(char **input, token_t tokens[], int *count, minishell_t *minishell)
{
    if (**input == '|')
        tokens[(*count)++] = (token_t){PIPE, ft_strndup("|", 1)};
    else if (**input == '<' && *(*input + 1) == '<')
        tokens[(*count)++] = (token_t){HEREDOC_IN, ft_strndup("<<", 2)}; 
    else if (**input == '>' && *(*input + 1) == '>')
        tokens[(*count)++] = (token_t){HEREDOC_OUT, ft_strndup(">>", 2)};
    else if (**input == '<')
        tokens[(*count)++] = (token_t){REDIRECT_IN,  ft_strndup("<", 1)};
    else if (**input == '>')
        tokens[(*count)++] = (token_t){REDIRECT_OUT, ft_strndup(">", 1)};
    else if (**input == '\"')
        double_quote(input, tokens, count, minishell);
    else if (**input == '\'')
        single_quote(input, tokens, count, minishell);
    else if (**input == '$' && *(*input + 1) == '?')
        tokens[(*count)++] = (token_t){ENV_QUEST, ft_strndup("$?", 2)};
    else
        process_word(input, tokens, count, minishell);
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
        if (ft_strncmp(tokens[count - 1].value, "<<", 3) == 0 
        || ft_strncmp(tokens[count - 1].value, ">>", 3) == 0 || ft_strncmp(tokens[count - 1].value, "$?", 3) == 0)
            input++;
        input++;
    }
    tokens[count] =  (token_t){TOKEN_END, "", 1};
    minishell->count = count;
}
