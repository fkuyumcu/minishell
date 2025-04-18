/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:32:13 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/14 19:12:34 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tree(ast_node_t *ast)
{
	int	i;

	i = 0;
	if (!ast)
		return ;

	if (ast->args && ast->args[i])
	{
		while (ast->args[i])
			free(ast->args[i++]);

	}
	if (ast->args)
		free(ast->args);
	free_tree(ast->left);
	free_tree(ast->right);
	free(ast);
}

int	count_token(token_t tokens[])
{
	int	i;

	i = 0;
	while (tokens[i].t_type != TOKEN_END)
		i++;
	return (i);
}

void	manage_tokens2(token_t tokens[], int i)
{
	while (tokens[i].t_type != TOKEN_END)
	{
		free(tokens[i].value);
		tokens[i].value = NULL;
		if (tokens[i + 1].t_type != TOKEN_END)
			tokens[i].value = ft_strdup(tokens[i + 1].value, tokens->ms);
		tokens[i].t_type = tokens[i + 1].t_type;
		tokens[i].is_dbl_quote = tokens[i + 1].is_dbl_quote;
		tokens[i].is_env = tokens[i + 1].is_env;
		tokens[i].is_equal = tokens[i + 1].is_equal;
		tokens[i].is_word = tokens[i + 1].is_word;
		tokens[i].index_in_input = tokens[i + 1].index_in_input;
		tokens[i].space_flag = tokens[i + 1].space_flag;
		i++;
	}
}

void	manage_tokens(token_t tk[])
{
	int	i;

	i = 0;
	while (tk[i].t_type != TOKEN_END)
	{
		while (tk[i].t_type == WORD && tk[i + 1].space_flag == 0
			&& (tk[i + 1].t_type == WORD || tk[i + 1].t_type == ENV_VAR))
		{
			tk[i].value = ft_strjoin(tk[i].value, tk[i + 1].value, tk->ms);
			manage_tokens2(tk, i + 1);
		}
		i++;
	}
}

void	free_tokens(token_t tokens[], minishell_t ms)
{
	int	i;

	i = 0;
	while (tokens[i].t_type != TOKEN_END)
		free (tokens[i++].value);
}

void	set_minishell(minishell_t *ms, int allocation, env_t *env_list,
		token_t tokens[])
{
	ms->allocation = allocation;
	ms->ast = NULL;
	ms->tokens = NULL;
	ms->input_start = NULL;
	ms->env_list = env_list;
	ms->tokens = tokens;
	
	
}

void	parser(minishell_t *ms, char *buf, int allocation)
{
	token_t	*tokens;
	env_t	*env_list;
	int		pos;

	allocation = ft_strlen(buf);
	ms->allocation = allocation;
	tokens = malloc(sizeof(token_t) * allocation);
	if (!tokens)
	return ;
	ms->env_list = env_list;
	lex_analize(buf, tokens, ms);
	check_env(tokens, ms);
	manage_tokens(tokens);

	ms->tokens = tokens;
	pos = 0;
	ms->size = count_token(tokens);
//	ms->ast = parse_expression(tokens, &pos, 0, ms);
	ms->line = create_line(ms);
	ms->mini_lines = split_for_pipe(ms->line, ms);
	line_t *cur = *ms->mini_lines;
	priority(ms);
/* 	printf("%d",ms->mini_lines[0]->priority);
	printf("%d",ms->mini_lines[1]->priority);
	printf("%d",ms->mini_lines[2]->priority); */
	//execute_pipeline(ms);

	pos = 0;
	while (ms->mini_lines[pos] != NULL)
		free_line(ms->mini_lines[pos++]);
	free(ms->mini_lines);
	free_line(ms->line);
	free_tokens(tokens, *ms);
	free(tokens);
}
