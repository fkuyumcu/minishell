/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analysis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:45:25 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/07 17:17:56 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_double_op(char **input, t_token tk[], int *count)
{
	if (**input == '<' && *(*input + 1) == '<')
	{
		tk[(*count)] = (t_token){HEREDOC_IN, ft_strndup("<<", 2, tk->ms), 0, 1,
			0, 0, 0, 0, 0, tk->ms, 0};
		(*count)++;
		(*input) += 2;
	}
	else if (**input == '>' && *(*input + 1) == '>')
	{
		tk[(*count)] = (t_token){HEREDOC_OUT, ft_strndup(">>", 2, tk->ms), 0, 1,
			0, 0, 0, 0, 0, tk->ms, 0};
		(*count)++;
		(*input) += 2;
	}
}

static void	process_single_op(char **input, t_token tk[], int *count)
{
	if (**input == '<')
	{
		tk[(*count)] = (t_token){REDIRECT_IN, ft_strndup("<", 1, tk->ms), 0, 1,
			0, 0, 0, 0, 0, tk->ms, 0};
		(*count)++;
		(*input)++;
	}
	else if (**input == '>')
	{
		tk[(*count)] = (t_token){REDIRECT_OUT, ft_strndup(">", 1, tk->ms), 0, 1,
			0, 0, 0, 0, 0, tk->ms, 0};
		(*count)++;
		(*input)++;
	}
}

void	process_op(char **input, t_token tk[], int *count)
{
	if (**input == '|')
	{
		tk[(*count)] = (t_token){PIPE, ft_strndup("|", 1, tk->ms), 0, 1, 0, 0,
			0, 0, 0, tk->ms, 0};
		(*count)++;
		(*input)++;
	}
	else if ((**input == '<' && *(*input + 1) == '<') || (**input == '>'
			&& *(*input + 1) == '>'))
		process_double_op(input, tk, count);
	else
		process_single_op(input, tk, count);
	while (is_whitespace(**input))
		(*input)++;
}

void	process_token(char **input, t_token tokens[], int *count,
		t_minishell *ms)
{
	if (**input == '|')
		process_op(input, tokens, count);
	else if (**input == '<' && *(*input + 1) == '<')
		process_op(input, tokens, count);
	else if (**input == '>' && *(*input + 1) == '>')
		process_op(input, tokens, count);
	else if (**input == '<')
		process_op(input, tokens, count);
	else if (**input == '>')
		process_op(input, tokens, count);
	else if (**input == '\"')
		double_quote(input, tokens, count, ms);
	else if (**input == '\'')
		single_quote(input, tokens, count, ms);
	else
		process_word(input, tokens, count, ms);
}

void	lex_analize(char *input, t_token *tokens, t_minishell *minishell)
{
	int	count;

	count = 0;
	if (input == NULL)
		return ;
	while (*input != '\0')
	{
		while (is_whitespace(*input))
			input++;
		if (*input == 0)
			break ;
		process_token(&input, tokens, &count, minishell);
	}
	tokens[count] = (t_token){TOKEN_END, "", 1, 0, 0, 0, 0, 0, 0, minishell, 0};
	minishell->count = count;
	check_syntax_errors(tokens, count, minishell);
}
