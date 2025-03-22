/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 15:22:01 by yalp              #+#    #+#             */
/*   Updated: 2025/03/22 16:19:56 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int find_heredoc(ast_node_t *ast, minishell_t *ms)
{
	if (!ast)
		return (0);

	if ((ast->left && ast->left->token == HEREDOC_IN) ||
	    (ast->right && ast->right->token == HEREDOC_IN))
	{
		return (1);
	}

	if (find_heredoc(ast->left, ms) || find_heredoc(ast->right, ms))
		return (1);

	return (0);
}