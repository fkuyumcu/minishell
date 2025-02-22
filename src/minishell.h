/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:59:10 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/22 14:35:39 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>

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
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_APPEND_IN,
    TOKEN_REDIRECT_APPEND_OUT,
    TOKEN_AMPERSAND,
    TOKEN_WORD,
    TOKEN_STRING,
    TOKEN_ENV_VAR,
    TOKEN_END
} token_type;


typedef struct s_env {
    char *key;
    char *value;
    struct s_env *next;
} env_t;


typedef struct token_s
{
    token_type t_type;
    char *value;
    size_t is_dbl_quote;
    size_t is_env;
    size_t is_equal;
} token_t;

typedef struct minishell_s
{
    env_t *env_list;
    int allocation;
    int flag;
    int count_token;
    char **env;
    env_t dollar_env;
    size_t count;
}
minishell_t;

void    ft_getcwd(char *buf, size_t size);
void    process_token(char **input, token_t tokens[], int *count, minishell_t *minishell);
void    lex_analize(char *input, token_t tokens[], minishell_t *minishell);
void	print_banner(void);
void	parser(char *buf);
char    *format_cwd(const char *cwd);
char    *ft_readline(void);
char	*ft_strncpy(char *dest, const char *src, size_t n);
size_t	ft_strlen(const char *s);
size_t	ft_strlen(const char *s);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
int     is_delimiter(char c);
char    *ft_strndup(const char *src, size_t n);
void    double_quote(char **input, token_t tokens[], int *count, minishell_t *minishell);
void    single_quote(char **input, token_t tokens[], int *count, minishell_t *minishell);
void    check_env(token_t tokens[], minishell_t *minishell);

#endif
