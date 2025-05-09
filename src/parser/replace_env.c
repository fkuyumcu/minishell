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

char	*prepare_replacement(char *src, char *new_value)
{
	size_t	new_len;
	size_t	new_value_len;
	size_t	count;
	size_t	old_len;
	char	*new_str;

	new_value_len = ft_strlen(new_value);
	old_len = ft_strlen(src);
	count = count_dollar_ask(src);
	if (count == 0)
		return (NULL);
	new_len = old_len + count * (new_value_len - 2) + 1;
	new_str = malloc(new_len);
	return (new_str);
}

void	replace_dollar_ask(char *new_str, char *src, char *new_value)
{
	char	*dst;
	size_t	new_value_len;

	dst = new_str;
	new_value_len = ft_strlen(new_value);
	while (*src)
	{
		if (src[0] == '$' && src[1] == '?')
		{
			ft_memcpy(dst, new_value, new_value_len);
			dst += new_value_len;
			src += 2;
		}
		else
			*dst++ = *src++;
	}
	*dst = '\0';
}

void	change_dollar_ask(t_token *token, char *new_value)
{
	char	*new_str;

	new_str = prepare_replacement(token->value, new_value);
	if (!new_str)
		return ;
	replace_dollar_ask(new_str, token->value, new_value);
	free(token->value);
	token->value = new_str;
}

void	replace_env_value(t_token *token, t_minishell *ms, char *dolar_pos,
		int len)
{
	char	*env_name;
	char	*env_value;
	char	*new_value;
	size_t	total_size;

	env_name = get_env_name(dolar_pos, ms, &len);
	if (!env_name)
		return ;
	env_value = find_list(ms, env_name);
	if (!env_value)
		env_value = getenv(env_name);
	if (!env_value)
		env_value = "";
	new_value = malloc(ft_strlen(token->value) - len + ft_strlen(env_value)
			+ 1);
	if (!new_value)
		ft_error(ms, "Error Allocating Space");
	ft_strncpy(new_value, token->value, dolar_pos - token->value);
	new_value[dolar_pos - token->value] = '\0';
	total_size = ft_strlen(token->value) - len + ft_strlen(env_value) + 1;
	ft_strlcat(new_value, env_value, total_size);
	ft_strlcat(new_value, dolar_pos + len + 1, total_size);
	free(token->value);
	token->value = new_value;
	free(env_name);
}
