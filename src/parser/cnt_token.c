/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cnt_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:08:04 by yalp              #+#    #+#             */
/*   Updated: 2025/05/09 09:02:08 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_token(t_token tokens[])
{
	int	i;

	i = 0;
	while (tokens[i].t_type != TOKEN_END)
		i++;
	return (i);
}

void	manage_tokens2(t_token tokens[], int i)
{
	while (tokens[i].t_type != TOKEN_END)
	{
		free(tokens[i].value);
		tokens[i].value = NULL;
		if (tokens[i + 1].t_type != TOKEN_END)
			tokens[i].value = ft_strdup(tokens[i + 1].value, tokens->ms);
		tokens[i].t_type = tokens[i + 1].t_type;
		tokens[i].is_dbl_quote = tokens[i + 1].is_dbl_quote;
		tokens[i].is_single_quote = tokens[i + 1].is_single_quote;
		tokens[i].is_env = tokens[i + 1].is_env;
		tokens[i].is_equal = tokens[i + 1].is_equal;
		tokens[i].is_word = tokens[i + 1].is_word;
		tokens[i].index_in_input = tokens[i + 1].index_in_input;
		tokens[i].space_flag = tokens[i + 1].space_flag;
		i++;
	}
}

void	manage_tokens(t_token tk[])
{
	int	i;

	i = 0;
	while (tk[i].t_type != TOKEN_END)
	{
		while (tk[i].t_type == WORD && tk[i + 1].space_flag == 0 && (tk[i
					+ 1].t_type == WORD || tk[i + 1].t_type == ENV_VAR))
		{
			tk[i].value = ft_strjoin(tk[i].value, tk[i + 1].value, tk->ms);
			manage_tokens2(tk, i + 1);
		}
		i++;
	}
}
