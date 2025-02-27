/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:59:10 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/24 15:34:17 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
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


typedef enum
{
    PIPE,
    HEREDOC_IN,
    HEREDOC_OUT,
    REDIRECT_IN,
    REDIRECT_OUT,
    WORD,
    ENV_VAR,
    ENV_QUEST,
    ENV_EQ,
    TOKEN_END
} token_type;


typedef struct ast_node_s {
    char **args;     
    struct ast_node_s *left;   
    struct ast_node_s *right;  
    token_type token;             
} ast_node_t;



typedef struct s_env {
    char *key;
    char *value;
    struct s_env *next;
} env_t;


typedef struct token_s
{
    token_type t_type;
    char *value;
    size_t space_flag;
    size_t is_dbl_quote;
    size_t is_env;
    size_t is_equal;
    size_t is_word;
    int index_in_input;
    
    
} token_t;

typedef struct minishell_s
{
    env_t *env_list;
    int allocation;
    int flag;
    int count_token;
    size_t count;
    char *input_start;
}
minishell_t;

void    process_token(char **input, token_t tokens[], int *count, minishell_t *minishell);
void    lex_analize(char *input, token_t tokens[], minishell_t *minishell);
void	print_banner(void);
void	parser(char *buf);
char    *ft_readline(void);
char	*ft_strncpy(char *dest, const char *src, size_t n);
size_t	ft_strlen(const char *s);
size_t	ft_strlen(const char *s);
char	*ft_strjoin(char *s1, char *s2);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
int     is_delimiter(char c);
char    *ft_strndup(const char *src, size_t n);
void    double_quote(char **input, token_t tokens[], int *count, minishell_t *minishell);
void    single_quote(char **input, token_t tokens[], int *count, minishell_t *minishell);
void    check_env(token_t tokens[], minishell_t *minishell);
char	*ft_strdup(const char *s1);
char	*ft_strchr(const char *s, int c);


void print_ast(ast_node_t *node, int level);
ast_node_t *create_ast_node(char **args, token_type type); 
int get_precedence(token_type type);
ast_node_t *parse_expression(token_t tokens[], int *pos, int size, int min_prec);
ast_node_t *parse_primary(token_t tokens[], int *pos, int size);
ast_node_t *parse_redirection(token_t tokens[], int *pos, int size, token_type redir_type);
char **collect_args(token_t tokens[], int *pos, int size);









#endif
