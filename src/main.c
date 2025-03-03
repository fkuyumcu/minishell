/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:59:05 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/03 13:54:16 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



char *ft_readline(void)
{
    char *buf;
    
    buf = NULL;
     
    buf = readline("pistonshell$>");
    if(buf)
        return (buf);
    
    return (NULL);
}

void sigint_handler(int sig)
{
    char cwd[BUFSIZ];
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void    sig_handler(int c)
{
    signal(SIGINT, &sigint_handler);
    signal(SIGQUIT, SIG_IGN);
}

void ft_exit(char *line)
{
    free (line);
	
    write(1, "exit", 4);
    exit(1);
}

int main(void)
{
    //print_banner();
    char *line;
    minishell_t minishell;
    while(1)
    {
        sig_handler(1);
        line = ft_readline();
        if(line == NULL)
            ft_exit(line);
        if(*line == '\0')
            continue;
        add_history(line);
        parser(minishell, line);
        /* if(line)leak var
            free(line); */
    }
    //wall wextra

    
}
