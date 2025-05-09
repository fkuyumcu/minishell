/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:35:38 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/07 16:43:22 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid(t_line *cur)
{
	if ((cur->type == REDIRECT_OUT || cur->type == HEREDOC_OUT) && (!cur->next
			|| cur->next->type != WORD))
		return (1);
	return (0);
}

static void	write_heredoc_loop(int pipefd, char *delimiter, bool do_expand)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		if (do_expand)
		{
			expanded = expand_variables(line);
			write(pipefd, expanded, strlen(expanded));
			free(expanded);
		}
		else
			write(pipefd, line, strlen(line));
		write(pipefd, "\n", 1);
		free(line);
	}
}

void	write_heredoc(t_line *heredoc_node, int pipefd[2])
{
	char	*delimiter;
	bool	do_expand;

	delimiter = heredoc_node->next->value;
	do_expand = !(heredoc_node->next->is_dbl_quote
			|| heredoc_node->next->is_single_quote);
	write_heredoc_loop(pipefd[1], delimiter, do_expand);
	close(pipefd[1]);
}

int	handle_single_heredoc(t_line *node, t_minishell *ms, int prev_read)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		ft_putstr_fd("minishell: pipe error\n", 2);
		check_and_free(ms, node, 0);
		exit(EXIT_FAILURE);
	}
	apply_heredoc_pipe(node, pipefd);
	if (prev_read != -1)
		close(prev_read);
	return (pipefd[0]);
}
