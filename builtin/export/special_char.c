/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:00:00 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/08 12:00:00 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../src/minishell.h"

int	has_special_chars(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '_' && !ft_isalnum(str[i]) && str[i] != '\0')
			return (1);
		i++;
	}
	return (0);
}

char	*prepare_quoted_value(char *arg, char *key)
{
	char	*equal_sign;
	char	*full_value;

	equal_sign = ft_strchr(arg, '=');
	full_value = NULL;
	if (!equal_sign)
		return (NULL);
	full_value = malloc(ft_strlen(arg) + 3);
	if (!full_value)
		return (NULL);
	ft_strncpy(full_value, key, ft_strlen(arg) + 3);
	ft_strlcat(full_value, "=", ft_strlen(arg) + 3);
	if (equal_sign[1] != '\0')
		ft_strlcat(full_value, equal_sign + 1, ft_strlen(arg) + 3);
	return (full_value);
}

static char	*create_full_value(char *key, char *value_part, char *arg)
{
	char	*full_value;

	if (*value_part != '\0' && has_special_chars(value_part))
	{
		full_value = malloc(ft_strlen(key) + 3 + ft_strlen(value_part) + 1);
		if (!full_value)
			return (NULL);
		ft_strncpy(full_value, key, ft_strlen(key) + 1);
		ft_strlcat(full_value, "=\"", ft_strlen(key) + 3 + ft_strlen(value_part)
			+ 1);
		ft_strlcat(full_value, value_part, ft_strlen(key) + 3
			+ ft_strlen(value_part) + 1);
		ft_strlcat(full_value, "\"", ft_strlen(key) + 3 + ft_strlen(value_part)
			+ 1);
	}
	else
	{
		full_value = malloc(ft_strlen(arg) + 1);
		if (!full_value)
			return (NULL);
		ft_strncpy(full_value, arg, ft_strlen(arg) + 1);
	}
	return (full_value);
}

char	*prepare_unquoted_value(char *arg, char *key)
{
	char	*equal_sign;
	char	*value_part;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (NULL);
	value_part = equal_sign + 1;
	return (create_full_value(key, value_part, arg));
}

char	*make_quoted_var(char *key, char *val, int is_dbl)
{
	char	*prefix;
	char	*suffix;
	size_t	newlen;
	char	*newvar;

	if (is_dbl)
	{
		prefix = "=\"";
		suffix = "\"";
	}
	else
	{
		prefix = "='";
		suffix = "'";
	}
	newlen = ft_strlen(key) + ft_strlen(prefix) + ft_strlen(val)
		+ ft_strlen(suffix) + 1;
	newvar = malloc(newlen);
	if (!newvar)
		return (NULL);
	ft_strncpy(newvar, key, ft_strlen(key) + 1);
	ft_strlcat(newvar, prefix, newlen);
	ft_strlcat(newvar, val, newlen);
	ft_strlcat(newvar, suffix, newlen);
	return (newvar);
}
