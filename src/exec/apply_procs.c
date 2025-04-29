/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_procs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:32:51 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/29 12:33:58 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_exec(line_t *cur, minishell_t *ms, int heredoc_fd)
{
    char	**args = malloc(sizeof(char *) * 100);
    int		ac = 0;
    char	*exec_path;

    fill_args(args, &ac, cur);

    if(ac == 0)//<< eof durumunda ana döngüye geri dönüyor. buradaki free kısmına dikkat et 
    {
        free(args);
        exit(1);
    }

    apply_redirections(cur, heredoc_fd);
    exec_path = get_exec_path(args, ms);
    if (!exec_path)
    {
        fprintf(stderr, "Command not found: %s\n", args[0]);
        free_args(args, ac);
        exit(127);
    }
    execve(exec_path, args, ms->envp);
    perror("execve");//perror
    free_args(args, ac);
    free(exec_path);
    exit(EXIT_FAILURE);
}

