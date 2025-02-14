/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:59:10 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/14 17:07:12 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>

# define RST   "\x1b[0m"
# define BLACK   "\x1b[30m"
# define RED     "\x1b[31m"
# define GREEN   "\x1b[32m"
# define YELLOW  "\033[38;2;255;215;0m"
# define BLUE    "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN    "\x1b[36m"
# define WHITE   "\x1b[37m"

# define MAX_TOKEN_LEN 20

typedef enum
{
    TOKEN_WORD,
    TOKEN_PIPE, 
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_AMPERSAND,
    TOKEN_END
} token_type;

typedef struct token_s
{
    token_type t_type;
    char value[100];
} token_t;


void    ft_getcwd(char *buf, size_t size);
char    *ft_readline(void);
void    lex_analize(char *input, token_t tokens[]);
void	print_banner(void);
void	parser(char *buf);
char	*ft_strncpy(char *dest, const char *src, size_t n);
size_t	ft_strlen(const char *s);

#endif