/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:59:05 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/15 16:46:45 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

char *format_cwd(const char *cwd)
{
    char *formatted_cwd = malloc(ft_strlen(cwd) + ft_strlen(GREEN) + ft_strlen(RST) + 4);//strlen

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
    size_t bufsize;
    char cwd[BUFSIZ];
    char *new;
    
    buf = NULL;
    
    ft_getcwd(cwd, sizeof(cwd));
    new = format_cwd(cwd);
    
    buf = readline(new);
    if(buf)
        return (buf);
    return (NULL);
}

int main(int argc, char **argv)
{
    //print_banner();
    char *line;
    while(1)
    {
        line = ft_readline();
        add_history(line);
        parser(line);
    }
    

    
}