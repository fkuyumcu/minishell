/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:32:13 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/09 09:57:57 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tokens(t_token tokens[])
{
	int	i;

	i = 0;
	while (tokens[i].t_type != TOKEN_END)
		free(tokens[i++].value);
}

void	set_minishell(t_minishell *ms, int allocation, t_env *env_list,
		t_token tokens[])
{
	ms->allocation = allocation;
	ms->tokens = NULL;
	ms->input_start = NULL;
	ms->env_list = env_list;
	ms->tokens = tokens;
}

static int	parser_init(t_minishell *ms, char *buf, t_token **tokens)
{
	int	allocation;

	allocation = ft_strlen(buf);
	ms->allocation = allocation;
	*tokens = malloc(sizeof(t_token) * allocation);
	if (!*tokens)
		return (0);
	lex_analize(buf, *tokens, ms);
	return (1);
}

static void	parser_exec(t_minishell *ms, t_token *tokens)
{
	int	pos;

	pos = 0;
	check_env(tokens, ms);
	manage_tokens(tokens);
	ms->tokens = tokens;
	ms->size = count_token(tokens);
	ms->line = create_line(ms);
	ms->mini_lines = split_for_pipe(ms->line, ms);
	priority(ms);
	if (ms->mini_lines[0])
		execute_pipeline(ms);
	pos = 0;
	while (ms->mini_lines[pos] != NULL)
		free_line(ms->mini_lines[pos++]);
	free(ms->mini_lines);
	free_line(ms->line);
}

void	parser(t_minishell *ms, char *buf)
{
	t_token	*tokens;

	ms->error = 0;
	if (!parser_init(ms, buf, &tokens))
		return ;
	if (ms->error != 1)
		parser_exec(ms, tokens);
	free_tokens(tokens);
	free(tokens);
}
