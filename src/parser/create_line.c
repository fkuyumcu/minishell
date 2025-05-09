/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:58:03 by yalp              #+#    #+#             */
/*   Updated: 2025/05/09 08:50:19 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_line	*copy_until_pipe(t_line **line, t_minishell *ms)
{
	t_line	*tmp;

	tmp = NULL;
	while (*line && (*line)->value && (*line)->value[0] == '\0')
		*line = (*line)->next;
	if (*line)
	{
		tmp = cpy_node(*line, ms);
		*line = (*line)->next;
		while (*line && (*line)->type != PIPE)
		{
			if ((*line)->value && (*line)->value[0] != '\0')
				ft_lstadd_back(&tmp, cpy_node(*line, ms));
			*line = (*line)->next;
		}
	}
	return (tmp);
}

t_line	**split_for_pipe(t_line *line, t_minishell *ms)
{
	t_line	**ret;
	int		i;

	i = 0;
	ret = malloc(sizeof(t_line *) * (count_pipe(ms) + 2));
	if (!ret)
		return (NULL);
	while (line)
	{
		ret[i++] = copy_until_pipe(&line, ms);
		if (line)
			line = line->next;
	}
	ret[i] = NULL;
	return (ret);
}

void	free_line(t_line *node)
{
	if (node)
	{
		if (node->next != NULL)
			free_line(node->next);
		if (node->value != NULL)
			free(node->value);
		free(node);
	}
}

t_line	*create_line_node(char *value, t_tokentype type, t_minishell *ms)
{
	t_line	*node;

	if (!value)
	{
		return (NULL);
	}
	node = NULL;
	node = malloc(sizeof(t_line));
	if (!node)
		return (NULL);
	node->value = ft_strdup(value, ms);
	node->type = type;
	node->is_dbl_quote = 0;
	node->is_single_quote = 0;
	node->next = NULL;
	return (node);
}

t_line	*create_line(t_minishell *ms)
{
	int		i;
	t_line	*node;
	t_line	*tmp;

	i = 1;
	node = create_line_node(ms->tokens[0].value, ms->tokens[0].t_type, ms);
	if (!node)
		return (NULL);
	node->is_dbl_quote = ms->tokens[0].is_dbl_quote;
	node->is_single_quote = ms->tokens[0].is_single_quote;
	while (ms->tokens[i].t_type != TOKEN_END)
	{
		tmp = create_line_node(ms->tokens[i].value, ms->tokens[i].t_type, ms);
		if (!tmp)
		{
			free_line(node);
			return (NULL);
		}
		tmp->is_dbl_quote = ms->tokens[i].is_dbl_quote;
		tmp->is_single_quote = ms->tokens[i].is_single_quote;
		ft_lstadd_back(&node, tmp);
		i++;
	}
	return (node);
}
