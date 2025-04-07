/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:56:43 by yalp              #+#    #+#             */
/*   Updated: 2025/04/07 15:18:31 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_realloc(char **args, size_t new_size, minishell_t *minishell)
{
	size_t	i;
	size_t	old_size;
	char	**tmp;

	i = -1;
	old_size = 0;
	if (args)
	{
		while (args[old_size])
			old_size++;
	}
	tmp = malloc((old_size + new_size + 1) * sizeof(char *));
	if (!tmp)
		ft_error(minishell, "Error Allocating Space");
	while (++i < old_size)
		tmp[i] = ft_strdup(args[i], minishell);
	i = 0;
	while (i < old_size)
		free(args[i++]);
	free(args);
	tmp[old_size + new_size] = NULL;
	return (tmp);
}

char	**collect_args(line_t *line, minishell_t *ms)
{
	char	**args;
	int		arg_count;
	line_t *head;

	head = line;
	arg_count = 0;
	args = NULL;
	if (ms->size == 1 && ft_strlen(line->value) == 1)
	{
		args = malloc(sizeof(char *) * 2);
		args[0] = ft_strdup(line->value, ms);
		line = line->next;
		args[1] = NULL;
		return (args);
	}
	while (line->next != NULL && line->type == WORD)
	{
		args = ft_realloc(args, 1, ms);
		args[arg_count] = ft_strdup(line->value, ms);
		line = line->next;
		arg_count++;
	}
	line = head;
	return (args);
}

void execute_mini_line(line_t *line, minishell_t *ms)
{
	line_t *head;
	int i;
	char **cmds;
	
	cmds = malloc(sizeof(char *) * 2);
	if (!cmds)
		return NULL;
	i = 0;
	cmds[i++] = ft_strdup(line->value, ms);
	head = line;
	while (line->type != WORD)
	{
		cmds = ft_realloc(cmds, 1, ms);
		line = line->next;
		cmds[i] = ft_strdup(line->value, ms);
	}
	
	line = head;
}

void	execute_pipes(minishell_t *ms)
{
	int		i;
	int		j;
	pid_t 	pid;
	int		*pipes[2];
	
	i = 0;
	pipes[2] = malloc(sizeof(int *)* ms->pipes);
	while (i <= ms->pipes)
	{
		if (pipe(pipes[i]) == -1)
        {
            perror("pipe failed");
            exit(1);
		}
	}
	i = 0;
	while (i <= ms->pipes)
	{
		pid = fork();
		if (pid == 0)
        {
            if (i > 0)
            {
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
            }
            if (i < ms->pipes)
            {
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][1]);
            }
		}
		j = 0;
		while (j <= ms->pipes)
		{
			close(pipes[j][0]);
			close(pipes[j++][1]);
		}
		
		execute_mini_line(ms->mini_lines[i], ms);
	}
}

void execute_line(minishell_t *ms)
{
	if (ms->pipes > 0)
	{
		execute_pipes(ms);
	}
	else
	{
		execute_mini_line(ms->line, ms);
	}
}
