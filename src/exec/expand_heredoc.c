/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 00:00:00 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/08 00:00:00 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	var_bounds(const char *line, size_t i, size_t *start, size_t *end)
{
	*start = i + 1;
	*end = *start;
	while (line[*end] && (ft_isalnum(line[*end]) || line[*end] == '_'))
		(*end)++;
}

static size_t	process_env_var(char *line, size_t i)
{
	size_t	start;
	size_t	end;
	char	*name;
	char	*val;
	size_t	len;

	var_bounds(line, i, &start, &end);
	name = ft_strndup(line + start, end - start, NULL);
	val = getenv(name);
	len = 0;
	if (val)
		len = ft_strlen(val);
	free(name);
	return (len);
}

static size_t	get_expanded_len(char *line)
{
	size_t	new_len;
	size_t	i;
	size_t	var_len;

	new_len = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && ft_isalpha(line[i + 1]))
		{
			var_len = process_env_var(line, i);
			new_len += var_len;
			while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'
					|| line[i] == '$'))
				i++;
		}
		else
		{
			new_len++;
			i++;
		}
	}
	return (new_len);
}

static void	copy_expanded(char *out, char *line, size_t *i, size_t *k)
{
	size_t	start;
	size_t	end;
	char	*name;
	char	*val;
	size_t	len;

	var_bounds(line, *i, &start, &end);
	name = ft_strndup(line + start, end - start, NULL);
	val = getenv(name);
	if (val)
	{
		len = ft_strlen(val);
		memcpy(out + *k, val, len);
		*k += len;
	}
	free(name);
	*i = end;
}

char	*expand_variables(char *line)
{
	size_t	i;
	size_t	k;
	char	*out;

	out = malloc(get_expanded_len(line) + 1);
	if (!out)
		return (NULL);
	i = 0;
	k = 0;
	while (line[i])
	{
		if (line[i] == '$' && ft_isalpha(line[i + 1]))
			copy_expanded(out, line, &i, &k);
		else
			out[k++] = line[i++];
	}
	out[k] = '\0';
	return (out);
}
