/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:59:05 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/09 14:50:16 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_processed_input(t_minishell *minishell)
{
	char	*line;
	char	*temp;

	temp = NULL;
	sig_handler();
	line = ft_readline();
	if (line == NULL)
		ft_exit(line, minishell);
	if (*line == '\0' || is_only_whitespace(line))
	{
		free(line);
		return (NULL);
	}
	temp = convert_tabs_to_spaces(line, minishell);
	free(line);
	line = temp;
	minishell->temp = temp;
	if (ft_strlen(line) == 1)
	{
		temp = ft_strjoin(line, " ", minishell);
		free(line);
		line = temp;
		minishell->temp = temp;
	}
	return (line);
}

static void	process_line(t_minishell *minishell)
{
	char	*line;

	while (1)
	{
		line = get_processed_input(minishell);
		if (line == NULL)
			continue ;
		add_history(line);
		parser(minishell, line);
		free(line);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	minishell;

	if (ac != 1)
	{
		ft_putstr_fd("Usage: ./minishell\n", 2);
		return (1);
	}
	minishell.ac = ac;
	minishell.av = av;
	minishell.temp = NULL;
	minishell.envp = envp;
	minishell.env_is_copy = 0;
	process_line(&minishell);
	clean_env(&minishell);
	return (0);
}
