/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:21:00 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/30 16:09:10 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	proc_eq(token_t *token, minishell_t *minishell)
{
	char	*start;
	char	*eq;
	env_t	*env;

	token->t_type = ENV_EQ;
	start = token->value;
	eq = ft_strchr(token->value, '=');

}

static char	*find_list(minishell_t *minishell, char *env_name)//find list fonksiyonuna bak
{
	env_t	*list;

	list = minishell->env_list;
	/* while(list)BUNA BAK	
	{
	    if (strcmp(list->key,env_name) == 0)
	        return (list->value);
	    list = list->next;        
	} */
	return (NULL);
}

static void	proc_env(token_t *token, minishell_t *minishell)
{
	char	*env_value;
	char	*env_name;
	char	*start;
	char	*dolar_pos;
	char	*new_value;
	int		len;
	size_t	total_size;

	len = 0;
	dolar_pos = ft_strchr(token->value, '$');
	if (!dolar_pos)
		return;
	start = dolar_pos + 1;
	while (start[len] && (isalnum(start[len]) || start[len] == '_' || start[len] == '?') )//isalnum
		len++;
	if (len == 0)
		return ;
	env_name = ft_strndup(start, len, minishell);
	if (strcmp(env_name, "?") == 0)
	{
		free(token->value);
		token->value = ft_itoa(global_code % 256);
		return ;
	}
	else
	{
		env_value = find_list(minishell, env_name);
	}
	env_value = find_list(minishell, env_name);//find list kısmına bak
	if (!env_value)
		env_value = getenv(env_name);
	if (!env_value)
		env_value = "";
	new_value = (char *)malloc(ft_strlen(token->value) - len + ft_strlen(env_value) + 1);
	if (!new_value)
		ft_error(minishell, "Error Allocating Space");
	ft_strncpy(new_value, token->value, dolar_pos - token->value);
	new_value[dolar_pos - token->value] = '\0';
	total_size = ft_strlen(token->value) - len + ft_strlen(env_value) + 1;
    ft_strlcat(new_value, env_value, total_size);
    ft_strlcat(new_value, dolar_pos + len + 1, total_size);
	free(token->value);
	token->value = new_value;	
	free(env_name);
	
}

void	check_env(token_t tk[], minishell_t *minishell)
{
	int	i;
	int	j;

	i = 0;
	while (i < minishell->count)
	{
		j = 0;
		if (ft_strchr(tk[i].value, '$') != 0 && tk[i].is_dbl_quote == 1) // $? işareti casei tekrar gözden geçirilmeli
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
		else if (ft_strchr(tk[i].value, '=') != 0)
			proc_eq(&tk[i], minishell);
		i++;
	}
}
