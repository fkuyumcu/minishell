/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:59:10 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/30 16:49:17 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

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

typedef struct line_s
{
	char				*value;
	token_type			type;
	struct line_s		*next;
	int					priority;
}						line_t;

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
	int					prec;
	size_t				is_word;
	struct minishell_s	*ms;
	int					index_in_input;

}						token_t;

typedef struct minishell_s
{
	token_t				*tokens;
	env_t				*env_list;
	int					allocation;
	int					flag;
	int					size;
	char				**envp;
	int					count_token;
	size_t				count;
	char				*input_start;
	line_t				*line;
	line_t				**mini_lines;
	int					pipes;
	int					error;
}						minishell_t;

// UTILITY FUNCTIONS
extern int				global_code;
char					*ft_itoa(int n);
char					*ft_strdup(const char *s1, minishell_t *minishell);
char					*ft_strchr(const char *s, int c);
char					*ft_readline(void);
char					*ft_strncpy(char *dest, const char *src, size_t n);
char					*ft_strndup(const char *src, size_t n,
							minishell_t *minishell);
char					*ft_strjoin(char *s1, char *s2, minishell_t *minishell);
size_t					ft_strlen(const char *s);
size_t					ft_strlen(const char *s);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
void					print_banner(void);
char					*ft_strnstr(const char *haystack, const char *needle,
							size_t len);
size_t					ft_strlcat(char *dst, const char *src, size_t n);

// TOKENIZER

void					process_token(char **input, token_t tokens[],
							int *count, minishell_t *minishell);
void					lex_analize(char *input, token_t tokens[],
							minishell_t *minishell);
void					parser(minishell_t *minishell, char *buf,
							int allocation);
int						is_delimiter(char c);
void					double_quote(char **input, token_t tokens[], int *count,
							minishell_t *minishell);
void					single_quote(char **input, token_t tokens[], int *count,
							minishell_t *minishell);
void					check_env(token_t tokens[], minishell_t *minishell);

// LINE
line_t					*create_line(minishell_t *ms);
void					free_line(line_t *node);
line_t					**split_for_pipe(line_t *line, minishell_t *ms);

// ERRORS AND FREES
void					free_tokens(token_t tokens[], minishell_t ms);
void					ft_error(minishell_t *minishell, char *s);
void					free_str_array(char **paths, int i);
void					free_args(char **args, int ac);

// EXECUTE

char					*get_exec_path(char **args, minishell_t *ms);
void					fill_args(char **args, int *ac, line_t *node,
							minishell_t *ms);
char					**ft_split(char const *s, char c);
void					ft_putstr_fd(char *s, int fd);
int						count_tokens(char const *s, char c);
void					*ft_memcpy(void *dest, const void *src, size_t n);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
char					*find(char *cmd, char **envp, minishell_t *minishell);
void					child_exec(line_t *cur, minishell_t *ms,
							int heredoc_fd);
void					apply_heredoc_pipe(line_t *heredoc_node, int pipefd[2],
							minishell_t *ms);
void					redir_in(line_t *cur);
void					redir_out(line_t *cur, int append);
void					apply_redirections(line_t *cmd, int heredoc_fd,
							minishell_t *ms);
int						handle_heredocs(line_t *line, minishell_t *ms);
void					priority(minishell_t *ms);
void					execute_pipeline(minishell_t *ms);

// BUILTINS
void					cd(line_t *ml);
void					echo(line_t *ml, minishell_t *ms);
void					export(line_t *ml, minishell_t *ms);
void					pwd(void);
void					env(minishell_t *ms);
void					ft_exit_2(line_t *ml);
void					unset(void);
bool					is_builtin(line_t *ml); // bool
void					try_builtins(line_t *ml, minishell_t *ms);

#endif
