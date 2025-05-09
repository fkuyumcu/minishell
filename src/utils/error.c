/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:35:07 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/07 15:35:02 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_error(t_minishell *minishell, char *s)
{
	if (minishell->tokens)
		free_tokens(minishell->tokens);
	printf("%s", s);
	exit (1);
}
