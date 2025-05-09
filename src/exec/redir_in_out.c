/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_redirects2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+ +#+           */
/*   Created: 2025/05/07 16:01:33 by yalp              #+#    #+#             */
/*   Updated: 2025/05/07 16:07:14 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_redirect(t_line *cur)
{
	t_line	*temp;

	while (cur->next)
	{
		if (cur->type == HEREDOC_IN)
		{
			temp = cur;
			while (temp->next)
			{
				if (is_valid(temp))
					return (0);
				temp = temp->next;
			}
		}
		if (is_valid(cur))
			return (0);
		cur = cur->next;
	}
	return (1);
}

int	check_redirect_heredoc(t_line *heredoc_node)
{
	t_line	*cur;

	cur = heredoc_node->next;
	while (cur)
	{
		if ((cur->type == REDIRECT_OUT || cur->type == HEREDOC_OUT))
		{
			if (!cur->next || cur->next->type != WORD)
				return (0);
		}
		cur = cur->next;
	}
	return (1);
}

void	redir_in(t_line *cur)
{
	int	fd;

	fd = open(cur->next->value, O_RDONLY);
	if (fd == -1)
	{
		if (access(cur->next->value, F_OK) == -1)
			ft_putstr_fd("minishell: No such file or directory\n", 2);
		else
			ft_putstr_fd("minishell: Permission denied\n", 2);
		check_and_free(NULL, cur, 0);
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	redir_out(t_line *cur, int append)
{
	int	fd;

	if (append)
		fd = open(cur->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cur->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		if (access(cur->next->value, F_OK) == -1)
			ft_putstr_fd("minishell: No such file or directory\n", 2);
		else
			ft_putstr_fd("minishell: Permission denied\n", 2);
		check_and_free(NULL, cur, 0);
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}
