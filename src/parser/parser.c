/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:32:13 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/07 14:47:45 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tree(ast_node_t *ast)
{
	int	i;

	i = 0;
	if (!ast)
		return ;
	if (ast->args)
	{
		while (ast->args[i])
			free(ast->args[i++]);
		free(ast->args);
	}
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
	/* printf("%s\n",tokens[0].value);
	printf("%s\n",tokens[1].value);
	printf("%s\n",tokens[2].value); */
	//printf("%s\n",tokens[0].value);
	pos = 0;
	ms->ast = parse_expression(tokens, &pos, count_token(tokens), 0, ms);
	print_ast(ms->ast, 0);
	free_tree(ms->ast);
	free_tokens(tokens, *ms);
	free(tokens);
}
