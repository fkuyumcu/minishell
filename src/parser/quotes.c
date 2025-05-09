/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:43:16 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/07 16:58:39 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	set_flag(char **input, t_token *tk, int *count, t_minishell *ms)
{
	if (*input == NULL || *input == ms->input_start)
		tk[*count].space_flag = 0;
	else if (*(*input - 1) == ' ')
		tk[*count].space_flag = 1;
	else
		tk[*count].space_flag = 0;
}

static int	handle_unclosed_quote(char *buffer, t_minishell *ms, int is_double)
{
	if (is_double)
	{
		printf("Error: Unclosed double quote\n");
		ms->error = 1;
	}
	else
		printf("Error: Unclosed single quote\n");
	free(buffer);
	return (1);
}

static int	fill_quote_buffer(char **input, char quote_char, char *buffer,
		int *buf_index)
{
	while (**input && **input != quote_char)
		dnwhile(quote_char, buffer, input, buf_index);
	return (**input == quote_char);
}

void	double_quote(char **input, t_token tk[], int *count, t_minishell *ms)
{
	char	*buffer;
	int		buf_index;

	buffer = malloc(sizeof(char) * ms->allocation);
	if (!buffer)
		return ;
	buf_index = 0;
	set_flag(input, tk, count, ms);
	(*input)++;
	if (!fill_quote_buffer(input, '\"', buffer, &buf_index))
	{
		handle_unclosed_quote(buffer, ms, 1);
		return ;
	}
	(*input)++;
	tk[*count].is_dbl_quote = 1;
	tk[*count].is_single_quote = 0;
	quote_supply(buffer, buf_index, count, tk);
	free(buffer);
	process_token(input, tk, count, ms);
}

void	single_quote(char **input, t_token tk[], int *count, t_minishell *ms)
{
	char	*buffer;
	int		buf_index;

	buffer = malloc(sizeof(char) * ms->allocation);
	if (!buffer)
		return ;
	buf_index = 0;
	set_flag(input, tk, count, ms);
	(*input)++;
	if (!fill_quote_buffer(input, '\'', buffer, &buf_index))
	{
		handle_unclosed_quote(buffer, ms, 0);
		return ;
	}
	(*input)++;
	tk[*count].is_dbl_quote = 0;
	tk[*count].is_single_quote = 1;
	quote_supply(buffer, buf_index, count, tk);
	free(buffer);
	process_token(input, tk, count, ms);
}
