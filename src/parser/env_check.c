/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:21:00 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/22 17:27:19 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *find_list(minishell_t *minishell, char *env_name)
{
    env_t *list;

    list = minishell->env_list;
    /* while(list)
    {
        if (strcmp(list->key,env_name) == 0)
            return (list->value);
        list = list->next;        
    } */
    return (NULL);
}



static void proc_env(token_t *token, minishell_t *minishell)
{
    char *env_value;
    char *env_name;
    
    token->t_type = ENV_VAR;
    env_name = token->value + 1;
    env_value = find_list(minishell, env_name);//potansiyel leak, env_list tarafından
     
    if (!env_value)
        env_value = getenv(env_name);
    if (env_value)
    {
        free(token->value);
        token->value = ft_strdup(env_value);
    }
    else
    {
        free(token->value);
        token->value = ft_strdup("");
    }
}
    
    



void check_env(token_t tokens[], minishell_t *minishell)
{
    int i;
    i = 0;
    while(i < minishell->count)
    {
        if(tokens[i].value[0] == '$' && tokens[i].value[1] != '?')
            proc_env(&tokens[i], minishell);
        i++;
    }
    
}


