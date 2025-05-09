/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpy_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:51:33 by yalp              #+#    #+#             */
/*   Updated: 2025/05/09 08:47:07 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_line	*cpy_node(t_line *line, t_minishell *ms)
{
	t_line	*ret;

	ret = NULL;
	ret = malloc(sizeof(t_line));
	ret->value = ft_strdup(line->value, ms);
	ret->type = line->type;
	ret->is_dbl_quote = line->is_dbl_quote;
	ret->is_single_quote = line->is_single_quote;
	ret->next = NULL;
	return (ret);
}

t_line	*ft_lstlast(t_line *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_back(t_line **lst, t_line *new)
{
	t_line	*tmp;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = ft_lstlast(*lst);
	tmp->next = new;
}

int	count_pipe(t_minishell *ms)
{
	int		ret;
	t_line	*tmp;

	ret = 0;
	tmp = ms->line;
	while (tmp != NULL)
	{
		if (tmp->type == PIPE)
			ret++;
		tmp = tmp->next;
	}
	return (ret);
}
