/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_word.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:18:07 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/15 14:04:07 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/wait.h>
#include <fcntl.h>

void free_str_array(char **paths, int i)
{
	while (paths[++i])
		free(paths[i]);
}

char *find(char *cmd, char **envp, minishell_t *minishell)
{
    char **paths;
    char *path;
    int i = 0;
    char *part_path;

    // PATH değişkeninin olup olmadığını kontrol et
    while (envp[i] && ft_strnstr(envp[i], "PATH", 4) == 0)
        i++;
    if (!envp[i])
        return NULL;  // PATH değişkeni yoksa NULL dön

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
		if (access(path, F_OK) != 0) // Dosya mevcutsa
			free(path);
        else if (access(path, F_OK) == 0) // Dosya mevcutsa
        {
			free(paths);
			free_str_array(paths, i);
            return path;
        }
        i++;
    }
	free_str_array(paths, i);
 	free(paths);
    return NULL;
}


void	executed(char *argv, char **envp, minishell_t *ms)
{
	char	**command;
	char	*path;

	command = ft_split(argv, ' ');
	path = find(command[0], envp, ms);
	if (path == 0)
	{
		//liberte(command);
        printf("Error");
		//error(ERR_CMD);
	}
	if (execve(path, command, envp) == -1)
		printf("Error");
}


void execute_word(ast_node_t *node, minishell_t *ms)
{
        char *cmd = node->args[0];
        char *exec_path = NULL;

        if (access(cmd, X_OK) == 0)
            exec_path = cmd;
        else
            exec_path = find(cmd, ms->envp, ms);

        if (!exec_path)
        {
            fprintf(stderr, "Error: Command not found: %s\n", cmd);
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) // Çocuk işlem
        {
            
            execve(exec_path, node->args, ms->envp);
            perror("execve failed"); 
            exit(EXIT_FAILURE);
        }
        else // Parent process
        {
            int status;
			free(exec_path);
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            {
                fprintf(stderr, "Error: Command execution failed: %s\n", cmd);
                exit(EXIT_FAILURE);
            }
        }
}



