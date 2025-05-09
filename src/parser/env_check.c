/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:21:00 by fkuyumcu          #+#             */
/*   Updated: 2025/05/07 17:13:18 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_name(const char *dolar_pos, t_minishell *minishell, int *len)
{
	int			i;
	const char	*start = dolar_pos + 1;

	i = 0;
	while (start[i] && dollar_chr(start[i]) && ft_isdigit(start[0]) == 0)
		i++;
	*len = i;
	if (i == 0)
		return (NULL);
	return (ft_strndup(start, i, minishell));
}

static void	proc_env(t_token *token, t_minishell *ms)
{
	char	*dolar_pos;
	int		len;
	char	*exit_code;

	len = 0;
	dolar_pos = ft_strchr(token->value, '$');
	if (dolar_pos && *(dolar_pos + 1) != '?')
		replace_env_value(token, ms, dolar_pos, len);
	if (ft_strnstr(token->value, "$?", ft_strlen(token->value)))
	{
		exit_code = ft_itoa(g_code % 256);
		change_dollar_ask(token, exit_code);
		free(exit_code);
	}
}

void	check_env(t_token tk[], t_minishell *minishell)
{
	size_t	i;
	int		j;

	i = 0;
	while (i < minishell->count)
	{
		j = 0;
		if (ft_strchr(tk[i].value, '$') != 0 && tk[i].is_single_quote == 0)
		{
			while (tk[i].value[j])
			{
				if (tk[i].value[j] == '$')
				{
					proc_env(&tk[i], minishell);
				}
				j++;
			}
		}
		i++;
	}
}
