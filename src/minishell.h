/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:59:10 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/09 08:49:12 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
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

typedef enum t_type_e
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
}						t_tokentype;

typedef struct s_line
{
	char				*value;
	t_tokentype			type;
	struct s_line		*next;
	int					priority;
	size_t				is_dbl_quote;
	size_t				is_single_quote;
}						t_line;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_token
{
	t_tokentype			t_type;
	char				*value;
	size_t				space_flag;
	size_t				is_dbl_quote;
	size_t				is_single_quote;
	size_t				is_env;
	size_t				is_equal;
	int					prec;
	size_t				is_word;
	struct s_minishell	*ms;
	int					index_in_input;

}						t_token;

typedef struct s_minishell
{
	int					ac;
	char				**av;
	t_token				*tokens;
	t_env				*env_list;
	int					allocation;
	int					flag;
	int					size;
	char				**envp;
	int					count_token;
	size_t				count;
	char				*input_start;
	t_line				*line;
	t_line				**mini_lines;
	int					pipes;
	pid_t				*pids;
	int					error;
	char				*temp;
	int					env_is_copy;
	char				**exec_args;
	int					exec_ac;
	char				*exec_path;
}						t_minishell;

// SIGNALS
void					sigint_handler(int a);
void					sig_handler(void);
char					*ft_readline(void);
void					ft_exit(char *line, t_minishell *minishell);

//ENV

char					*prepare_replacement(char *src, char *new_value);
void					replace_dollar_ask(char *new_str, char *src,
							char *new_value);
void					change_dollar_ask(t_token *token, char *new_value);
void					replace_env_value(t_token *token, t_minishell *ms,
							char *dolar_pos, int len);
char					*get_env_name(const char *dolar_pos,
							t_minishell *minishell,
							int *len);

void					free_envp_ms(t_minishell *ms);

// UTILS

extern int				g_code;
char					*convert_tabs_to_spaces(char *input, t_minishell *ms);
int						is_only_whitespace(char *str);
void					clean_env(t_minishell *ms);
void					process_word(char **inp, t_token tokens[], int *count,
							t_minishell *ms);
int						is_whitespace(char c);
int						check_syntax_errors(t_token *tokens, int count,
							t_minishell *ms);
void					wordfill(t_token tk[], int *count, char *buf,
							t_minishell *ms);
int						dollar_chr(char c);
char					*find_list(t_minishell *minishell, char *env_name);
size_t					count_dollar_ask(char *str);
void					proc_eq(t_token *token, t_minishell *minishell);
void					manage_tokens(t_token tk[]);
void					manage_tokens2(t_token tokens[], int i);
int						count_token(t_token tokens[]);
void					setup_pipe(int i, int cmd_count, int *pipefd,
							int **curr_pipe);
int						cmd_counter(t_line **mini_lines);
void					wait_all(pid_t *pids, int cmd_count);
void					handle_path_exec(t_minishell *ms, int i, int *pipefd,
							int prev_fd);
void					handle_builtin(t_minishell *ms, int i, int *pipefd,
							int prev_fd);
void					bi_parent(t_minishell *ms, int i, int heredoc_fd);
void					bi_child(t_minishell *ms, int i, int *pipefd,
							int prev_fd);
void					init_pids(t_minishell *ms, int cmd_count);
char					*minidup(char *str);
void					dnwhile(char quo, char *buffer, char **input,
							int *buf_index);
void					quote_supply(char *buffer, int buf_index, int *count,
							t_token *tk);
t_line					*cpy_node(t_line *line, t_minishell *ms);
t_line					*ft_lstlast(t_line *lst);
void					ft_lstadd_back(t_line **lst, t_line *new);
int						count_pipe(t_minishell *ms);
void					free_tokens_end(t_minishell *ms);
void					write_heredoc(t_line *heredoc_node, int pipefd[2]);
int						is_valid(t_line *cur);
int						handle_single_heredoc(t_line *node, t_minishell *ms,
							int prev_read);
int						handle_heredocs(t_line *line, t_minishell *ms);
int						check_redirect(t_line *cur);
void					apply_redirections(t_line *cmd, int heredoc_fd);
void					apply_heredoc_pipe(t_line *heredoc_node, int pipefd[2]);
int						execute_command(t_line *cmd, t_minishell *ms);
int						check_redirect_heredoc(t_line *heredoc_node);
void					redir_in(t_line *cur);
void					redir_out(t_line *cur, int append);
void					free_lines(t_minishell *ms, t_line *cur);
void					free_args_exec(char **args, int ac, char *exec_path);
void					close_fds(void);
void					ft_puterror(t_line *cur, t_minishell *ms, char *str);
void					free_main_line(t_minishell *ms, t_line *cur);
void					free_pids(t_minishell *ms);
int						count_args(t_line *cur);
int						is_directory(char *path);
char					*ft_itoa(int n);
char					*ft_strdup(const char *s1, t_minishell *minishell);
char					*ft_strchr(const char *s, int c);
char					*ft_readline(void);
char					*ft_strncpy(char *dest, const char *src, size_t n);
char					*ft_strndup(const char *src, size_t n,
							t_minishell *minishell);
char					*ft_strjoin(char *s1, char *s2, t_minishell *minishell);
size_t					ft_strlen(const char *s);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
void					print_banner(void);
char					*ft_strnstr(const char *haystack, const char *needle,
							size_t len);
size_t					ft_strlcat(char *dst, const char *src, size_t n);
int						ft_isalnum(int c);
int						ft_isalpha(int c);
int						ft_isdigit(int c);

// TOKENIZER

void					process_token(char **input, t_token tokens[],
							int *count, t_minishell *minishell);
void					lex_analize(char *input, t_token tokens[],
							t_minishell *minishell);
void					parser(t_minishell *minishell, char *buf);
int						is_delimiter(char c);
void					double_quote(char **input, t_token tokens[], int *count,
							t_minishell *minishell);
void					single_quote(char **input, t_token tokens[], int *count,
							t_minishell *minishell);
void					check_env(t_token tokens[], t_minishell *minishell);
void					check_and_free(t_minishell *ms, t_line *cur,
							int update_args);

// LINE
t_line					*create_line(t_minishell *ms);
void					free_line(t_line *node);
t_line					**split_for_pipe(t_line *line, t_minishell *ms);

// ERRORS AND FREES
void					free_tokens(t_token tokens[]);
void					ft_error(t_minishell *minishell, char *s);
void					free_str_array(char **paths, int i);
void					free_args(char **args, int ac);

// EXECUTE

char					*get_exec_path(char **args, t_minishell *ms);
void					fill_args(char **args, int *ac, t_line *node,
							t_minishell *ms);
char					**ft_split(char const *s, char c);
void					ft_putstr_fd(char *s, int fd);
int						count_tokens(char const *s, char c);
void					*ft_memcpy(void *dest, const void *src, size_t n);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
char					*find(char *cmd, char **envp, t_minishell *minishell);
void					child_exec(t_line *cur, t_minishell *ms,
							int heredoc_fd);
void					redir_in(t_line *cur);
void					redir_out(t_line *cur, int append);

int						handle_heredocs(t_line *line, t_minishell *ms);
void					priority(t_minishell *ms);
void					execute_pipeline(t_minishell *ms);
void					ll_to_args(char **args, int *ac, t_line *node,
							t_minishell *ms);

// BUILTINS
void					cd(t_line *ml, t_minishell *ms);
void					echo(t_line *ml);
void					export(t_line *ml, t_minishell *ms);
void					pwd(void);
void					env(t_minishell *ms);
void					bi_exit(t_line *ml, t_minishell *ms);
void					unset(t_line *ml, t_minishell *ms);
bool					is_builtin(t_line *ml);
void					try_builtins(t_line *ml, t_minishell *ms);

char					*get_env(char *arg);
int						find_env_index(t_minishell *ms, char *key);
char					**alloc_new_env(int size);
void					copy_env_vars(char **dest, char **src, int size,
							t_minishell *ms);

void					free_env_array(char **env);
void					replace_env_var(t_minishell *ms, int idx, char *arg);
void					add_env_var(t_minishell *ms, char *arg, int size);
int						is_valid_identifier(t_line *ml);

int						has_special_chars(const char *str);
char					*prepare_quoted_value(char *arg, char *key);
char					*prepare_unquoted_value(char *arg, char *key);
char					*make_quoted_var(char *key, char *val, int is_dbl);

void					display_sorted_env(t_minishell *ms);
void					handle_quoted_export(t_line **current, t_minishell *ms);
void					export_loop(t_line *current, t_minishell *ms);

char					*expand_variables(char *line);

// UTILS ETC (utils_etc.c)
void					free_filtered(t_line *head);
t_line					*copy_line(t_line *src, t_minishell *ms);
void					filter_args_loop(t_line **ml, t_line **prev,
							t_line **current, t_minishell *ms);

//CD
char					*get_env_value(char **env, char *name);
int						cd_find_env_index(char **env, char *key);
char					**create_new_envp(char **old_envp, int size);
void					free_envp(char **envp);
void					set_new_var(t_minishell *ms, char *new_var, int size);
char					*create_env_string(t_minishell *ms, char *name,
							char *value);
void					update_env_var(t_minishell *ms, char *name,
							char *value);
char					*get_current_directory(void);
void					update_pwd_and_oldpwd(t_minishell *ms, char *old_pwd,
							char *new_pwd);
int						cd_to_home(t_minishell *ms);
int						cd_to_previous(t_minishell *ms);
int						cd_to_path(char *path, char *old_pwd);
int						handle_cd_args(t_line *ml, t_minishell *ms,
							char *old_pwd);

#endif
