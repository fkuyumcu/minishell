/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:32:13 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/01 15:41:24 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_tree(ast_node_t *ast)
{
	int i = 0;
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
	int i = 0;
	while (tokens[i].t_type != TOKEN_END)
		i++;
	return (i);
}

void manage_tokens2(token_t tokens[], int i)
{
	while (tokens[i].t_type != TOKEN_END)
	{
		free(tokens[i].value);
		tokens[i].value = NULL;
		if (tokens[i + 1].t_type != TOKEN_END)
			tokens[i].value = ft_strdup(tokens[i+1].value);
		tokens[i].t_type = tokens[i+1].t_type;
		tokens[i].is_dbl_quote = tokens[i+1].is_dbl_quote;
		tokens[i].is_env = tokens[i+1].is_env;
		tokens[i].is_equal = tokens[i+1].is_equal;
		tokens[i].is_word = tokens[i+1].is_word;
		tokens[i].index_in_input = tokens[i+1].index_in_input;
		tokens[i].space_flag = tokens[i+1].space_flag;
		i++;
	}
}

void manage_tokens(token_t tokens[])
{
	int i = 0;

	while (tokens[i].t_type != TOKEN_END)
	{
		while (tokens[i].t_type == WORD &&
			   (tokens[i + 1].t_type == WORD || tokens[i + 1].t_type == ENV_VAR) &&
			   tokens[i + 1].space_flag == 0)
		{
			tokens[i].value = ft_strjoin(tokens[i].value, tokens[i + 1].value);
			manage_tokens2(tokens, i + 1);
		}
		i++;
	}
}


void    free_tokens(token_t tokens[], minishell_t ms)
{
  int i = 0;
  while (tokens[i].t_type != TOKEN_END)
    free(tokens[i++].value);
}

void parser(char *buf)
{
    minishell_t minishell;
    int allocation;
    env_t *env_list;
    allocation = ft_strlen(buf);

    minishell.allocation = allocation;
    minishell.env_list = env_list;

    token_t tokens[allocation * sizeof(token_t)];
    lex_analize(buf, tokens, &minishell);
	
    check_env(tokens, &minishell);
	
	manage_tokens(tokens);
	
	/* printf("%s\n",tokens[0].value);
	printf("%s\n",tokens[1].value);
	printf("%s\n",tokens[2].value); */


	 int pos = 0;
	minishell.ast = parse_expression(tokens, &pos, count_token(tokens), 0);

   	print_ast(minishell.ast, 0); 
	free_tree(minishell.ast);
 
    free_tokens(tokens, minishell);
  
}
