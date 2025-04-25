/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:56:43 by yalp              #+#    #+#             */
/*   Updated: 2025/04/25 15:46:51 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_last_redirect(line_t *line)
{
	line_t *head;
	
	head = line;
	while (line->next != NULL)
	{
		line = line->next;
		if (line->type == HEREDOC_OUT || line->type == REDIRECT_OUT)
			{
				line = head;
				return(0);
			}
	}
	line = head;
	return (1);
}
void set_and_run_redirects(char **routs, char** appends, line_t *line)
{
	line_t *head;
	int i;
	int j;

	i = 0;
	j = 0;
	head = line;
	while (line->next != NULL)
	{
		if (line->type == REDIRECT_OUT)
		{
			routs[i++] = line->next->value;	
			if (is_last_redirect(line) == 1)
				{
					run_old_redirects(routs, appends);
					run_last_redirect(line->next->value, 0);
				}
		}
		if (line->type == HEREDOC_OUT)
		{
			appends[j++] = line->next->value;	
			if (is_last_redirect(line) == 1)
				{
					run_old_redirects(routs, appends);
					run_last_redirect(line->next->value, 1);
				}
		}
	}
}


void execute_redirects(line_t *line, minishell_t *ms)
{
	line_t *head;
	int r_outs;
	int appends;
	char **routs;
	char **appends1;
	
	r_outs = 0;
	appends = 0;
	head = line;
	while (line->next != NULL)
	{
		if (line->type == REDIRECT_OUT)
			r_outs++;
		if (line->type == HEREDOC_OUT)
			appends++;
	}
	routs = malloc(sizeof(char *) * r_outs + 1);
	appends1 = malloc(sizeof(char *) * appends);
	line = head;
	set_and_run_redirects(routs, appends1, line)
}



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
