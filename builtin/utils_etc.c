/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 03:48:21 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/03 12:42:28 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

void	free_filtered(t_line *head)
{
	t_line	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->value);
		free(temp);
	}
}

static bool	is_redirect_type(int type)
{
	return (type == REDIRECT_IN || type == REDIRECT_OUT || type == HEREDOC_IN
		|| type == HEREDOC_OUT);
}

t_line	*copy_line(t_line *src, t_minishell *ms)
{
	t_line	*new;

	new = NULL;
	new = malloc(sizeof(t_line));
	if (!new)
		return (NULL);
	new->value = ft_strdup(src->value, ms);
	new->type = src->type;
	new->next = NULL;
	new->priority = src->priority;
	new->is_dbl_quote = src->is_dbl_quote;
	new->is_single_quote = src->is_single_quote;
	return (new);
}

static bool	should_add_arg(t_line *ml, t_line *prev)
{
	if (!is_redirect_type(ml->type) && !(prev && is_redirect_type(prev->type)))
		return (true);
	return (false);
}

void	filter_args_loop(t_line **ml, t_line **prev, t_line **current,
		t_minishell *ms)
{
	t_line	*temp;

	while (*ml)
	{
		if (should_add_arg(*ml, *prev))
		{
			temp = copy_line(*ml, ms);
			if (!temp)
				return ;
			(*current)->next = temp;
			*current = temp;
		}
		*prev = *ml;
		*ml = (*ml)->next;
	}
}
