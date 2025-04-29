/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_procs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:32:51 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/29 15:51:24 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int count_args(line_t *cur)
{
    int count;
    
    count = 0;
    line_t *tmp = cur;
    while (tmp && tmp->value)
    {
        count++;
        tmp = tmp->next;
    }
    return count;
}
void	child_exec(line_t *cur, minishell_t *ms, int heredoc_fd)
{
    int count;
    char **args;
    int		ac = 0;
    char	*exec_path;
    
    args = malloc(sizeof(char *) * count_args(cur));
    
    fill_args(args, &ac, cur, ms);

    if(ac == 0)//<< eof durumunda ana döngüye geri dönüyor. buradaki free kısmına dikkat et 
    {
        free(args);
        exit(1);
    }

    apply_redirections(cur, heredoc_fd, ms);
    exec_path = get_exec_path(args, ms);
    if (!exec_path)
    {
        printf("Command not found: %s\n", args[0]);
        free_args(args, ac);
        exit(1);
    }
    execve(exec_path, args, ms->envp);
    printf("execve");
    free_args(args, ac);
    free(exec_path);
    exit(EXIT_FAILURE);//exit
}

