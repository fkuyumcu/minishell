/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:38:07 by yalp              #+#    #+#             */
/*   Updated: 2025/05/07 17:38:37 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_env(t_minishell *ms)
{
	int	i;

	if (ms->env_is_copy && ms->envp)
	{
		i = 0;
		while (ms->envp[i])
		{
			free(ms->envp[i]);
			ms->envp[i] = NULL;
			i++;
		}
		free(ms->envp);
		ms->envp = NULL;
	}
}

int	is_only_whitespace(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

char	*convert_tabs_to_spaces(char *input, t_minishell *ms)
{
	char	*result;
	int		i;
	int		len;

	if (!input)
		return (NULL);
	len = ft_strlen(input);
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		ft_error(ms, "Error Allocating Space");
	i = 0;
	while (input[i])
	{
		if (input[i] == '\t')
			result[i] = ' ';
		else
			result[i] = input[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
