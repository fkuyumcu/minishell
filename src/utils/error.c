/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:35:07 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/04 15:18:49 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_error(minishell_t *minishell, char *s)
{
     if(minishell->tokens)
        free_tokens(minishell->tokens, *minishell);
     if(minishell->ast)
        free_tree(minishell->ast);
    printf("%s",s);
    exit(1);
}
