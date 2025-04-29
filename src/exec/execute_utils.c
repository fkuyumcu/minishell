/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:31:14 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/29 15:52:31 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *find(char *cmd, char **envp, minishell_t *minishell)
{
    char **paths;
    char *path;
    int i = 0;
    char *part_path;

    while (envp[i] && ft_strnstr(envp[i], "PATH", 4) == 0)
        i++;
    if (!envp[i])
        return NULL;

    paths = ft_split(envp[i] + 5, ':');
    if (!paths)
        return NULL;
    i = 0;
    while (paths[i])
    {
        part_path = ft_strjoin(paths[i], "/",minishell);
        if (!part_path)
            break;
        path = ft_strjoin(part_path, cmd ,minishell);
        if (!path)
            break;
		if (access(path, F_OK) != 0)
			free(path);
        else if (access(path, F_OK) == 0)
        {
			free_str_array(paths, i);
			free(paths);
            return path;
        }
        i++;
    }
	free_str_array(paths, i);
 	free(paths);
    return (NULL);
}


void	fill_args(char **args, int *ac, line_t *node, minishell_t *ms)
{
    while (node)
    {
        if (node->type == REDIRECT_IN || node->type == REDIRECT_OUT
            || node->type == HEREDOC_IN || node->type == HEREDOC_OUT)
        {
            node = node->next;
            if (node)
                node = node->next;
            continue;
        }
        if (node->type == WORD)
            args[(*ac)++] = ft_strdup(node->value, ms);
        node = node->next;
    }
    args[*ac] = NULL;
}

char	*get_exec_path(char **args, minishell_t *ms)
{
    char	*exec_path;

    exec_path = NULL;
    if (args[0] && access(args[0], X_OK) == 0)
        exec_path = ft_strdup(args[0], ms);
    else if (args[0])
        exec_path = find(args[0], ms->envp, ms);
    return (exec_path);
}