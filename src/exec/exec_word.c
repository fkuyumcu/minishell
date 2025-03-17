/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_word.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:18:07 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/17 13:28:38 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/wait.h>
#include <fcntl.h>

void free_str_array(char **paths, int i)
{
	if (paths[i])
	{
		while (paths[++i])
			free(paths[i]);
	}

}

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
        //free(part_path);
        if (!path)
            break;
		if (access(path, F_OK) != 0)
			free(path);
        else if (access(path, F_OK) == 0)
        {
			free(paths);
			free_str_array(paths, i);
            return path;
        }
        i++;
    }
	free_str_array(paths, i);
 	free(paths);
    return (NULL);
}


void	executed(char *argv, char **envp, minishell_t *ms)
{
	char	**command;
	char	*path;

	command = ft_split(argv, ' ');
	path = find(command[0], envp, ms);
	if (path == 0)
	{
        printf("Error");
	}
	if (execve(path, command, envp) == -1)
		printf("Error");
}


void execute_word(ast_node_t *node, minishell_t *ms)
{
        char *cmd = node->args[0];
        char *exec_path = NULL;
        int status;
        pid_t pid;
        if (access(cmd, X_OK) == 0)
            exec_path = cmd;
            
        else
            exec_path = find(cmd, ms->envp, ms);
            
        if (!exec_path)
            printf("Error: Command not found: %s\n", cmd);//error

        pid = fork();
        if (pid == -1)
           printf("error");//err
        else if (pid == 0)   
        {
            execve(exec_path, node->args, ms->envp);
            free(exec_path);
        }
        else
        {
			free(exec_path);
            waitpid(pid, &status, 0);
        }
}
