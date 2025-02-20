/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:32:13 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/20 13:15:33 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void parser(char *buf)
{
    minishell_t minishell;
    env_t *env_list;
    minishell.env_list = env_list;
    token_t tokens[MAX_TOKEN_LEN];//bak
    lex_analize(buf, tokens, &minishell);
    printf("%s", tokens[0].value);
    //printf("%s", tokens[1].value);
    //printf("%s", tokens[2].value);
    //printf("%s", tokens[3].value);
}