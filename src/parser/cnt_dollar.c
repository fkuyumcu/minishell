/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cnt_dollar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:08:16 by yalp              #+#    #+#             */
/*   Updated: 2025/05/09 12:25:02 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	count_dollar_ask(char *str)
{
	size_t	count;
	char	*tmp;

	count = 0;
	tmp = str;
	while (tmp)
	{
		tmp = ft_strnstr(tmp, "$?", ft_strlen(tmp));
		if (tmp)
		{
			count++;
			tmp += 2;
		}
	}
	return (count);
}

char	*find_list(t_minishell *minishell, char *env_name)
{
	int		i;
	int		key_len;
	char	*value_start;

	key_len = ft_strlen(env_name);
	i = 0;
	while (minishell->envp[i])
	{
		if (ft_strncmp(minishell->envp[i], env_name, key_len) == 0
			&& (minishell->envp[i][key_len] == '='
			|| minishell->envp[i][key_len] == '\0'))
		{
			value_start = ft_strchr(minishell->envp[i], '=');
			if (value_start)
				return (value_start + 1);
			else
				return ("");
		}
		i++;
	}
	return (NULL);
}

int	dollar_chr(char c)
{
	if (ft_isalnum(c) == 1 || c == '_')
		return (1);
	return (0);
}
