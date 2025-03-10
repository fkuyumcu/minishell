/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:59:05 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/10 16:14:26 by yalp             ###   ########.fr       */
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
	line = NULL;
    minishell_t minishell;
    while(1)
    {
        sig_handler(1);
        line = ft_readline();
        if(line == NULL)
            ft_exit(line);
        if(*line == '\0')
		{
			free(line);
            continue;
		}
		if(strlen(line) == 1)
			line = ft_strjoin(line, " ", NULL);
        add_history(line);
        parser(&minishell, line, ft_strlen(line));
        //execute(minishell)
		free(line);

    }
    //wall wextra

    
}
