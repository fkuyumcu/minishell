/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:59:05 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/20 18:38:11 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <signal.h>

char *format_cwd(const char *cwd)
{
    char *formatted_cwd = malloc(ft_strlen(cwd) + ft_strlen(GREEN) + ft_strlen(RST) + 4);//malloc

    if (!formatted_cwd)
        return NULL;

    int i = 0;
    const char *green_ptr = GREEN;
    const char *rst_ptr = RST;
    
    while (*green_ptr)
        formatted_cwd[i++] = *green_ptr++;
    while (*cwd)
        formatted_cwd[i++] = *cwd++;
    formatted_cwd[i++] = '$';
    formatted_cwd[i++] = '>';
    formatted_cwd[i++] = ' ';

    
    while (*rst_ptr)
        formatted_cwd[i++] = *rst_ptr++;
    formatted_cwd[i] = '\0';

    return formatted_cwd;
}



char *ft_readline(void)
{
    char *buf;
    char cwd[BUFSIZ];
    char *new;
    
    buf = NULL;
    
    ft_getcwd(cwd, sizeof(cwd));
    new = format_cwd(cwd);
    
    buf = readline(new);
    //free(new);
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

void ft_exit()
{
    free (line);
    write(1, "exit", 4);
    exit(1);
}

int main(void)
{
    //print_banner();
    char *line;
    while(1)
    {
        sig_handler(1);
        line = ft_readline();
        if(line == NULL)
            ft_exit();
        add_history(line);
        //space durumunda seg
        parser(line);
        free(line);
    }
    //wall wextra

    
}
