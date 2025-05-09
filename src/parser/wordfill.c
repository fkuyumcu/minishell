/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordfill.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:08:30 by yalp              #+#    #+#             */
/*   Updated: 2025/05/07 17:18:42 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wordfill(t_token tk[], int *count, char *buf, t_minishell *ms)
{
	tk[*count].is_word = 1;
	tk[*count].is_dbl_quote = 0;
	tk[*count].is_single_quote = 0;
	tk[*count].t_type = WORD;
	tk[*count].value = ft_strdup(buf, ms);
	(*count)++;
}

int	check_syntax_errors(t_token *tokens, int count, t_minishell *ms)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if ((tokens[i].t_type == REDIRECT_IN || tokens[i].t_type == REDIRECT_OUT
				|| tokens[i].t_type == HEREDOC_IN
				|| tokens[i].t_type == HEREDOC_OUT || tokens[i].t_type == PIPE)
			&& (i + 1 >= count || tokens[i + 1].t_type != WORD))
		{
			write(2,
				"minishell: syntax error near unexpected token `newline'\n",
				56);
			ms->error = 1;
			return (1);
		}
		i++;
	}
	return (0);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

void	process_word(char **inp, t_token tokens[], int *count, t_minishell *ms)
{
	char	*buffer;
	int		buf_index;

	buffer = NULL;
	buffer = malloc(sizeof(char) * (ms->allocation + 1));
	if (!buffer)
		return ;
	buf_index = 0;
	if (*(*inp - 1) == ' ' || *(*inp - 1) == '\t')
		tokens[*count].space_flag = 1;
	else
		tokens[*count].space_flag = 0;
	while (**inp && ft_strchr(" \t|<>\"'", **inp) == 0)
	{
		buffer[buf_index++] = **inp;
		(*inp)++;
	}
	buffer[buf_index] = '\0';
	if (buf_index > 0)
		wordfill(tokens, count, buffer, ms);
	free(buffer);
	if (ft_strchr("|<>\"'", **inp) != 0 && **inp)
		process_token(inp, tokens, count, ms);
}
