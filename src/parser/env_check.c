/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:21:00 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/01 16:37:49 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void change_dollar_ask(token_t *token, char *new_value)
{
    char *dollar_pos;
    char *new_str;
    size_t prefix_len, new_value_len, suffix_len, total_len;

    dollar_pos = strstr(token->value, "$?");
    if (!dollar_pos)
        return;

    prefix_len = dollar_pos - token->value;
    new_value_len = ft_strlen(new_value);
    suffix_len = ft_strlen(dollar_pos + 2);
    total_len = prefix_len + new_value_len + suffix_len + 1;

    new_str = malloc(total_len);
    if (!new_str)
        return;

    ft_strncpy(new_str, token->value, prefix_len);
    new_str[prefix_len] = '\0';
    ft_strlcat(new_str, new_value, total_len);
    ft_strlcat(new_str, dollar_pos + 2, total_len);

    free(token->value);
    token->value = new_str;
}


void	proc_eq(token_t *token, minishell_t *minishell)
{
	char	*start;
	char	*eq;
	env_t	*env;

	token->t_type = ENV_EQ;
	start = token->value;
	eq = ft_strchr(token->value, '=');

}

static char	*find_list(minishell_t *minishell, char *env_name)
{
    int		i;
    int		key_len;
    char	*value_start;

    key_len = ft_strlen(env_name);
    i = 0;
    while (minishell->envp[i])
    {
        if (ft_strncmp(minishell->envp[i], env_name, key_len) == 0 && 
            (minishell->envp[i][key_len] == '=' || minishell->envp[i][key_len] == '\0'))
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
		change_dollar_ask(token, ft_itoa(global_code));
		return ;
	}
	else
	{
		env_value = find_list(minishell, env_name);
	}
	env_value = find_list(minishell, env_name);
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
		if (ft_strchr(tk[i].value, '$') != 0 && tk[i].is_dbl_quote == 1)
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
