/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:58:03 by yalp              #+#    #+#             */
/*   Updated: 2025/03/26 16:42:26 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int count_pipe(minishell_t *ms)
{
	int ret;
	line_t *head;

	ret = 0;
	head = ms->line;
	while (ms->line->next != NULL)
	{
		if (ms->line->type == PIPE)
			ret++;
		ms->line = ms->line->next;
	}
	ms->line = head;
	return (ret);
}
line_t **split_for_pipe(line_t *line, minishell_t *ms)
{
    line_t **ret;
    int i;
	line_t *tmp;

	ret = malloc(sizeof(line_t *) * ( count_pipe(ms) + 2));
	if (!ret)
		return (NULL);
	i = 0;
    while (line)
    {
        ret[i++] = line;
        while (line && line->type != PIPE)
            line = line->next;
        if (line)
        {
            line_t *tmp = line;
            line = line->next;
            tmp->next = NULL;
			free(tmp);
        }
    }
    ret[i] = NULL;
    return ret;
}



void free_line(line_t *node)
{
	if (node->next != NULL)
		free_line(node->next);
	if (node->value)
		free(node->value);
	free(node);
}

line_t	*ft_lstlast(line_t *lst)
{
	if (!lst)
		return (NULL);
	while (lst ->next != NULL)
	{
		lst = lst -> next;
	}
	return (lst);
}

void	ft_lstadd_back(line_t **lst, line_t *new)
{
	line_t	*tmp;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = ft_lstlast(*lst);
	tmp -> next = new;
}

line_t *create_line_node(char *value, token_type type, minishell_t *ms)
{
	line_t *node = malloc(sizeof(line_t));
	if (!node)
		return (NULL);
	node->value = ft_strdup(value, ms);
	node->type = type;
	node->next = NULL;
	return (node);
}

line_t	*create_line(minishell_t *ms)
{
	int	i;

	i = 1;
	line_t *node = NULL;
	line_t *tmp = NULL;
	node = create_line_node(ms->tokens[0].value, ms->tokens[0].t_type, ms);
	while (ms->tokens[i].t_type != TOKEN_END)
	{
		tmp = create_line_node(ms->tokens[i].value, ms->tokens[i].t_type, ms);
		ft_lstadd_back(&node, tmp);
		i++;
	}
	return (node);
}
