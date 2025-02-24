/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analysis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:45:25 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/24 15:16:14 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

ast_node_t *create_node(token_type type, char **args)
{
    ast_node_t *node = malloc(sizeof(ast_node_t));
    if (!node) return NULL;
    node->token = type;
    node->args = args;
    node->left = NULL;
    node->right = NULL;
    return node;
}

char **edit_command_args(token_t *tokens, int *pos, int size)
{
    int start;
    int count; 
    char **args;
    int i;

    start = 0;
    *pos = 0;
    count = 0;
    i = -1;

    while(*pos < size && tokens[*pos].t_type == WORD)
    {
        (*pos)++;
        count ++;
    }
    args = malloc(sizeof(char *) * (count + 1));
    if(!args)
        return (NULL);
    while(++i < count)
        args[i] = tokens[start + i].value;
    args[count] = NULL;
    return (args);
}
