/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:59:05 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/07 18:26:13 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_readline(void)
{
	char	*buf;

	buf = NULL;
	buf = readline("pistonshell$>");
	if (buf)
		return (buf);
	return (NULL);
}

void	sigint_handler(int a)
{
	(void)a;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	g_code = 130;
	rl_redisplay();
}

void	sig_handler(void)
{
	signal(SIGINT, &sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_exit(char *line, t_minishell *minishell)
{
	free(line);
	clean_env(minishell);
	write(1, "exit\n", 5);
	exit(0);
}
