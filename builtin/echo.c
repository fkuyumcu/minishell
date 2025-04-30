/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:10:21 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/30 16:14:15 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

int	is_flag_n(const char *arg)
{
    int i = 1;
    if (!arg || arg[0] != '-' || arg[1] != 'n')
        return 0;
    while (arg[i])
    {
        if (arg[i] != 'n')
            return 0;
        i++;
    }
    return 1;
}

// Ortam değişkeninin değerini bul
static char *get_env_value(const char *var_name, minishell_t *ms)
{
    int i = 0;
    size_t len;
    
    if (!var_name || !ms)
        return NULL;
        
    len = strlen(var_name);
    while (ms->envp[i])
    {
        if (strncmp(ms->envp[i], var_name, len) == 0 && ms->envp[i][len] == '=')
            return ms->envp[i] + len + 1;  // '=' karakterinden sonrasını döndür
        i++;
    }
    return NULL;
}

static char *expand_variables(const char *str, minishell_t *ms)//dolar işaretini handle etmeye bak
{
    char *result = NULL;
    char *var_name = NULL;
    char *var_value = NULL;
    int i = 0, j = 0, start;
    int max_size = strlen(str) * 2;
    
    result = malloc(max_size);
    if (!result)
        return NULL;
    
    while (str[i])
    {
        if (str[i] == '$' && str[i+1] && str[i+1] != ' ')
        {
            i++;  // $ işaretini atla
            start = i;
            
            // Değişken adını al
            while (str[i] && ((str[i] >= 'A' && str[i] <= 'Z') || 
                             (str[i] >= 'a' && str[i] <= 'z') ||
                             (str[i] >= '0' && str[i] <= '9') ||
                              str[i] == '_'))
                i++;
            
            var_name = strndup(str + start, i - start);
            if (!var_name)
                continue;
            
            // Değişken değerini bul ve ekle
            var_value = get_env_value(var_name, ms);
            if (var_value)
            {
                strcpy(result + j, var_value);
                j += strlen(var_value);
            }
            
            free(var_name);
        }
        else
        {
            result[j++] = str[i++];
        }
    }
    
    result[j] = '\0';
    return result;
}


void	echo(line_t *ml, minishell_t *ms)
{
    int print_newline = 1;
    line_t *cur;
    char *expanded_value;
    
    cur = ml->next; // ilk node komutun kendisi (echo)
    
    while (cur && is_flag_n(cur->value))
    {
        print_newline = 0;
        cur = cur->next;
    }
    
    while (cur)
    {
        expanded_value = expand_variables(cur->value, ms);
        if (expanded_value)
        {
            printf("%s", expanded_value);
            free(expanded_value);
        }
        else
            printf("%s", cur->value);
            
        if (cur->next)
            printf(" ");
        cur = cur->next;
    }
    
    if (print_newline)
        printf("\n");
}
