/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:55:30 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/30 15:59:02 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int	is_valid_identifier(char *str)
{
    int	i = 0;

    if (!str || !(str[0]) || (!(str[0] >= 'A' && str[0] <= 'Z') &&
        !(str[0] >= 'a' && str[0] <= 'z') && str[0] != '_'))
        return (0);
    while (str[i] && str[i] != '=')
    {
        if (!(str[i] >= 'A' && str[i] <= 'Z') &&
            !(str[i] >= 'a' && str[i] <= 'z') &&
            !(str[i] >= '0' && str[i] <= '9') && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

static void update_envp(char *arg, minishell_t *ms)
{
    int     i;
    size_t  key_len;

    key_len = 0;
    while (arg[key_len] && arg[key_len] != '=')
        key_len++;
    i = 0;
    while (ms->envp[i])
    {
        if (!strncmp(ms->envp[i], arg, key_len) && 
            (ms->envp[i][key_len] == '=' || ms->envp[i][key_len] == '\0'))
        {
            free(ms->envp[i]);  // Eski değeri serbest bırak
            ms->envp[i] = strdup(arg);
            return;
        }
        i++;
    }
    // Yeni değişken ekle
    char **new_envp = malloc(sizeof(char *) * (i + 2));
    if (!new_envp)
        return;
    for (int j = 0; j < i; j++)
        new_envp[j] = ms->envp[j];
    new_envp[i] = strdup(arg);
    new_envp[i + 1] = NULL;
    free(ms->envp);  // Eski envp'yi serbest bırak
    ms->envp = new_envp;
}

// Mevcut ortam değişkenlerini alfabetik sıraya göre listele
static void display_sorted_env(minishell_t *ms)
{
    int i = 0, j, len;
    char **sorted_env;
    char *temp;

    // Ortam değişkeni sayısını bul
    while (ms->envp[i])
        i++;
    
    // Ortam değişkenlerini kopyala
    sorted_env = malloc(sizeof(char *) * (i + 1));
    if (!sorted_env)
        return;
    
    for (j = 0; j < i; j++)
        sorted_env[j] = strdup(ms->envp[j]);
    sorted_env[i] = NULL;
    
    // Kabarcık sıralaması ile sırala
    for (j = 0; j < i - 1; j++)
    {
        for (int k = 0; k < i - j - 1; k++)
        {
            if (strcmp(sorted_env[k], sorted_env[k + 1]) > 0)
            {
                temp = sorted_env[k];
                sorted_env[k] = sorted_env[k + 1];
                sorted_env[k + 1] = temp;
            }
        }
    }
    
    // Sıralanmış değişkenleri yazdır
    for (j = 0; j < i; j++)
        printf("declare -x %s\n", sorted_env[j]);
    
    // Belleği temizle
    for (j = 0; j < i; j++)
        free(sorted_env[j]);
    free(sorted_env);
}

void export(line_t *ml, minishell_t *ms)
{
    line_t *current = ml->next;
    
    // Argüman yoksa mevcut ortam değişkenlerini listele
    if (!current)
    {
        display_sorted_env(ms);
        return;
    }
    
    // Argümanları işle
    while (current)
    {
        if (!is_valid_identifier(current->value))
        {
            printf("minishell: export: `%s': not a valid identifier\n", current->value);
        }
        else
        {
            // '=' içersin veya içermesin her geçerli tanımlayıcıyı ekle
            update_envp(current->value, ms);
        }
        current = current->next;
    }
}