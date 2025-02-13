/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:59:05 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/13 16:26:10 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>



char *ft_readline(void)
{
    char *buf;
    size_t bufsize;
    char cwd[BUFSIZ];
    
    buf = NULL;
    ft_getcwd(cwd, sizeof(cwd));
    printf(GREEN"%s>$"RST, cwd);
    buf = readline("");
    if(buf)
        return (buf);
    return (NULL);
}

int main(int argc, char **argv)
{

    char *line;

    while(1)
    {
        line = ft_readline();
        printf("%s\n", line);   
        
    //REPL
    }
    

    
}