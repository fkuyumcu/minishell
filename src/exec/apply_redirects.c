/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_redirects3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:01:30 by yalp              #+#    #+#             */
/*   Updated: 2025/05/07 16:07:33 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_heredocs(t_line *line, t_minishell *ms)
{
	t_line	*tmp;
	int		prev_read;

	tmp = line;
	prev_read = -1;
	while (tmp)
	{
		if (tmp->type == HEREDOC_IN)
			prev_read = handle_single_heredoc(tmp, ms, prev_read);
		tmp = tmp->next;
	}
	return (prev_read);
}

void	apply_redirections(t_line *cmd, int heredoc_fd)
{
	t_line	*cur;

	cur = cmd;
	while (cur)
	{
		if ((cur->type == REDIRECT_IN || cur->type == REDIRECT_OUT
				|| cur->type == HEREDOC_OUT) && cur->next)
		{
			if (cur->type == REDIRECT_IN)
				redir_in(cur);
			else
				redir_out(cur, cur->type == HEREDOC_OUT);
			cur = cur->next;
		}
		cur = cur->next;
	}
	if (heredoc_fd != -1)
	{
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
	}
}

void	apply_heredoc_pipe(t_line *heredoc_node, int pipefd[2])
{
	int	can_write;

	can_write = check_redirect_heredoc(heredoc_node);
	if (can_write)
		write_heredoc(heredoc_node, pipefd);
	else
		close(pipefd[1]);
}
