/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:55:27 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/29 14:42:20 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_pipeline(minishell_t *ms)
{
    int		cmd_count;
    pid_t	*pids;
    int     i;
    int		pipefd[2];
    int		prev_fd;
    int     heredoc_fd;
    
    
    i = 0;
    cmd_count = 0;
    prev_fd = -1;
    
    while (ms->mini_lines[cmd_count])
        cmd_count++;
        
    pids = malloc(sizeof(pid_t) * cmd_count);//pids dizisinde miniline sayısı kadar pid için yer a.
    while (i < cmd_count)//her bir miniline için while döngüsünün içinde dolaş
    {
        heredoc_fd = handle_heredocs(ms->mini_lines[i], ms);//her döngüde handle_heredoc fonksiyonundan gelen dönüş değerini heredoun yazma ucu olarak tut
        if (i < cmd_count - 1)
            pipe(pipefd);//miniline sonda değilse pipe aç
            
        pids[i] = fork();//her bir miniline için process oluştur
        if (pids[i] == 0)
        {
            if (i > 0)//eğer ilk miniline değilse, bir önceki miniline'ın yazma ucuyla stdin'i değiştir
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);//bir önceki miniline'ın yazma ucunu kapat
            }
            if (i < cmd_count - 1)//son miniline'da değilsek pipe'ın yazma ucuna std çıktıyı yönlendir 
            {
                close(pipefd[0]);//pipe'ın okuma kafasını kapat
                dup2(pipefd[1], STDOUT_FILENO);//pipe'ın diğer ucuyla stdout'u değiştir
                close(pipefd[1]);//yazma ucunu kapat
            }
            apply_redirections(ms->mini_lines[i], heredoc_fd, ms);//yönlendirmeleri uygula
            child_exec(ms->mini_lines[i], ms, -1); 
        }
        if (i > 0)
            close(prev_fd);
        if (i < cmd_count - 1)
        {
            close(pipefd[1]);
            prev_fd = pipefd[0];
        }
        if (heredoc_fd != -1)
            close(heredoc_fd);
        i++;
    }
    i = 0;
    while (i < cmd_count)
        waitpid(pids[i++], NULL, 0);
    free(pids);
}

void	priority(minishell_t *ms)
{
    int		max_priority;
    int		i;
    line_t	*current;

    i = 0;
    while (ms->mini_lines[i])
    {
        max_priority = 0;
        current = ms->mini_lines[i];
        while (current != NULL)
        {
            if (current->value != NULL)
            {
                if (ft_strncmp("<<", current->value, 2) == 0
                    || ft_strncmp(">>", current->value, 2) == 0)
                {
                    max_priority = 2;
                    break;
                }
                else if ((ft_strncmp("<", current->value, 1) == 0
                        || ft_strncmp(">", current->value, 1) == 0)
                    && max_priority < 1)
                    max_priority = 1;
            }
            current = current->next;
        }
        if (ms->mini_lines[i] != NULL)
            ms->mini_lines[i]->priority = max_priority;
        i++;
    }
}

