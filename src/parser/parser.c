/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:32:13 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/22 18:19:01 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    free_tokens(token_t tokens[], minishell_t ms)
{
  int i = 0;
  while (tokens[i].t_type != TOKEN_END)
    free(tokens[i++].value);
}

void parser(char *buf)
{
     minishell_t minishell;
    int c;
    int in_word;
    int in_quotes;
    int allocation;
    env_t *env_list;
    c = 0;
    in_word = 0;
    in_quotes = 0;
    minishell.count_token = 0;
    allocation = ft_strlen(buf);
    minishell.allocation = allocation;
    minishell.env_list = env_list;
    token_t tokens[allocation * sizeof(token_t)];
    lex_analize(buf, tokens, &minishell);
    check_env(tokens, &minishell);
    printf("%s\n", tokens[0].value);
    printf("%s\n", tokens[1].value);
    printf("%s\n", tokens[2].value);
    free_tokens(tokens, minishell);
  
}
