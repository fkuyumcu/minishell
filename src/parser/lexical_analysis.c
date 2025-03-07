/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analysis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:45:25 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/07 14:44:52 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_op(char **input, token_t tk[], int *count)
{
	if (**input == '|')
	{
		tk[(*count)++] = (token_t){PIPE, ft_strndup("|", 1, tk->ms)};
		(*input)++;
	}
	else if (**input == '<' && *(*input + 1) == '<')
	{
		tk[(*count)++] = (token_t){HEREDOC_IN, ft_strndup("<<", 2, tk->ms)};
		(*input) += 2;
	}
	else if (**input == '>' && *(*input + 1) == '>')
	{
		tk[(*count)++] = (token_t){HEREDOC_OUT, ft_strndup(">>", 2, tk->ms)};
		(*input) += 2;
	}
	else if (**input == '<')
	{
		tk[(*count)++] = (token_t){REDIRECT_IN, ft_strndup("<", 1, tk->ms)};
		(*input)++;
	}
	else if (**input == '>')
	{
		tk[(*count)++] = (token_t){REDIRECT_OUT, ft_strndup(">", 1, tk->ms)};
		(*input)++;
	}
}

void	wordfill(token_t tk[], int *count, char *buf)
{
	tk[*count].is_word = 1;
	tk[*count].is_dbl_quote = 1;
	tk[*count].t_type = WORD;
	tk[*count].value = strdup(buf);
	(*count)++;
}

void	process_word(char **inp, token_t tokens[], int *count, minishell_t *ms)
{
	char	*buffer;
	int		buf_index;

	buffer = malloc(sizeof(char) * (ms->allocation + 1));
	if (!buffer)
		return ;
	buf_index = 0;
	if (*(*inp - 1) == ' ')
		tokens[*count].space_flag = 1;
	else
		tokens[*count].space_flag = 0;
	while (**inp && ft_strchr(" |<>\"'", **inp) == 0)
	{
		buffer[buf_index++] = **inp;
		(*inp)++;
	}
	buffer[buf_index] = '\0';
	if (buf_index > 0)
		wordfill(tokens, count, buffer);
	free(buffer);
	if (ft_strchr("|<>\"'", **inp) != 0 && **inp)
		process_token(inp, tokens, count, ms);
}

void	process_token(char **input, token_t tokens[], int *count,
	minishell_t *ms)
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
	else if (**input == '$' && *(*input + 1) == '?')
		tokens[(*count)++] = (token_t){ENV_QUEST, ft_strndup("$?", 2,
				tokens->ms)};
	else
		process_word(input, tokens, count, ms);
}				

void	lex_analize(char *input, token_t *tokens, minishell_t *minishell)
{
	int	count;

	count = 0;
	if (input == NULL)
		return ;
	while (*input != '\0')
	{
		while (*input == ' ')
			input++;
		if (*input == 0)
			break ;
		process_token(&input, tokens, &count, minishell);
	}
	tokens[count] = (token_t){TOKEN_END, "", 1};
	minishell->count = count;
}
