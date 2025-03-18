/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:57:33 by yalp              #+#    #+#             */
/*   Updated: 2025/03/17 17:17:20 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

void	handle_heredoc(char *delimiter, char **command, char **envp, minishell_t *minishell)
{
	int		pipefd[2];
	pid_t	pid;
	char	*line;
	char	*cmd_path;
	size_t	len;

	if (pipe(pipefd) == -1)
	{
		ft_putstr_fd("pipe error\n", 2);
		exit(1);
	}
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("fork error\n", 2);
		exit(1);
	}
	if (pid == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		cmd_path = find(command[0], envp, minishell);
		if (!cmd_path)
		{
			ft_putstr_fd("command not found\n", 2);
			exit(127);
		}
		execve(cmd_path, command, envp);
		ft_putstr_fd("execve error\n", 2);
		exit(1);
	}
	close(pipefd[0]);
	line = NULL;
	len = 0;
	while (1)
	{
		ft_putstr_fd("> ", 1);
		if (getline(&line, &len, stdin) == -1)
			break ;
		line[strcspn(line, "\n")] = '\0'; //strcspn kullanıldı
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1); // Yeni satır eklemeyi unutma
	}
	free(line);
	close(pipefd[1]);
	wait(NULL);
}