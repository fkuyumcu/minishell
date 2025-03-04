/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:18:07 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/04 16:38:01 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int check_ast(ast_node_t *node)
{
    if(!node)
        return (1);
    if(node->token == PIPE)
    {
        if(!(node->left) || !(node->right))
        {
            printf("Invalid pipe usage\n");
            return (0);
        }
    }
    if (node->token == REDIRECT_IN || node->token == REDIRECT_OUT || node->token == HEREDOC_OUT || node->token == HEREDOC_IN)
    {
        if (!node->right || !node->right->args || !node->right->args[0]) 
        {
            printf("Syntax error: missing file for redirection\n");
            return (0);
        }
    }
    if (node->token == WORD && (!node->args || !node->args[0]))
    {
        printf("Syntax error: empty command\n");
        return (0);
    }
    return (check_ast(node->left) && check_ast(node->right));    
}

void execute(minishell_t *minishell)
{
    
}