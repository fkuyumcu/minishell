/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:59:10 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/22 16:14:33 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>



# define RST "\x1b[0m"
# define BLACK "\x1b[30m"
# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define YELLOW "\033[38;2;255;215;0m"
# define BLUE "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN "\x1b[36m"
# define WHITE "\x1b[37m"

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
}						token_type;

typedef struct ast_node_s
{
	char				**args;
	struct ast_node_s	*left;
	struct ast_node_s	*right;
	token_type			token;
}						ast_node_t;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						env_t;

typedef struct token_s
{
	token_type			t_type;
	char				*value;
	size_t				space_flag;
	size_t				is_dbl_quote;
	size_t				is_env;
	size_t				is_equal;
	int prec;
	size_t				is_word;
    struct minishell_s  *ms;
	int					index_in_input;

}						token_t;

typedef struct minishell_s
{
	token_t				*tokens;
	env_t				*env_list;
	int					allocation;
	int					flag;
	int size;
	char **envp;
	int					count_token;
	size_t				count;
	char				*input_start;
	ast_node_t			*ast;
}						minishell_t;

//UTILITY FUNCTIONS

char					*ft_strdup(const char *s1, minishell_t *minishell);
char					*ft_strchr(const char *s, int c);
char					*ft_readline(void);
char					*ft_strncpy(char *dest, const char *src, size_t n);
char					*ft_strndup(const char *src, size_t n, minishell_t *minishell);
char					*ft_strjoin(char *s1, char *s2, minishell_t *minishell);
size_t					ft_strlen(const char *s);
size_t					ft_strlen(const char *s);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
void					print_banner(void);

//TOKENIZER

void					process_token(char **input, token_t tokens[],
							int *count, minishell_t *minishell);
void					lex_analize(char *input, token_t tokens[],
							minishell_t *minishell);
void					parser(minishell_t *minishell, char *buf, int allocation);
int						is_delimiter(char c);
void					double_quote(char **input, token_t tokens[], int *count,
							minishell_t *minishell);
void					single_quote(char **input, token_t tokens[], int *count,
							minishell_t *minishell);
void					check_env(token_t tokens[], minishell_t *minishell);

//CREATE AST

void					print_ast(ast_node_t *node, int level);
ast_node_t				*create_ast_node(char **args, token_type type,
							minishell_t *minishell);
ast_node_t				*parse_expression(token_t tokens[], int *pos,
							int min_prec, minishell_t *minishell);
ast_node_t				*parse_primary(token_t tokens[], int *pos,
							minishell_t *minishell);
ast_node_t				*parse_redirection(token_t tokens[], int *pos,
							token_type redir_type, minishell_t *minishell);							
char					**collect_args(token_t tokens[], int *pos, 
											minishell_t *minishell);
int						get_precedence(token_type type);

//ERRORS AND FREES

void					free_tokens(token_t tokens[], minishell_t ms);
void					free_tree(ast_node_t *ast);
void					ft_error(minishell_t *minishell, char *s);

//EXECUTE

void	execute(minishell_t *minishell);
char	*find_executable(char *cmd);
void	execute_word(ast_node_t *node, minishell_t *ms);
void	execute_ast(ast_node_t *node, minishell_t *ms);
char	**ft_split(char const *s, char c);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
void	ft_putstr_fd(char *s, int fd);
int	count_tokens(char const *s, char c);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char *find(char *cmd, char **envp, minishell_t *minishell);
void execute_heredoc(ast_node_t *node, minishell_t *ms, char *out);
void	handle_heredoc(char *delimiter, ast_node_t *node, minishell_t *minishell, char *out);
int find_heredoc(ast_node_t *ast, minishell_t *ms);


#endif
