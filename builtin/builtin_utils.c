/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:05:44 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/01 15:12:48 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

void free_filtered_list(line_t *head)
{
    line_t *temp;
    
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp->value);
        free(temp);
    }
}

line_t *filter_redirects(line_t *ml)
{
    line_t *head = ml;
    line_t *temp = NULL;
    line_t *new_head = NULL;
    line_t *current = NULL;
    line_t *previous_token = NULL;
    

    temp = malloc(sizeof(line_t));
    if (!temp)
        return NULL;
    temp->value = strdup(ml->value);
    temp->type = ml->type;
    temp->next = NULL;
    temp->priority = ml->priority;
    
    new_head = temp;
    current = temp;
    
    // Argümanları ekleyelim, yönlendirmeleri ve dosya adlarını atlayalım
    previous_token = ml; // İlk token'ı sakla
    ml = ml->next;
    
    while (ml)
    {
        if (ml->type != REDIRECT_IN && ml->type != REDIRECT_OUT && 
            ml->type != HEREDOC_IN && ml->type != HEREDOC_OUT)
        {
            // Bir önceki token yönlendirme mi kontrol et
            if (previous_token && (previous_token->type == REDIRECT_IN || 
                previous_token->type == REDIRECT_OUT || 
                previous_token->type == HEREDOC_IN || 
                previous_token->type == HEREDOC_OUT))
            {
                // Bu token yönlendirmenin hedefi, atlayalım
            }
            else
            {
                // Normal argüman, ekleyelim
                temp = malloc(sizeof(line_t));
                if (!temp)
                    return new_head; // Hata durumunda en azından buraya kadar olan listeyi döndür
                
                temp->value = strdup(ml->value);
                temp->type = ml->type;
                temp->next = NULL;
                temp->priority = ml->priority;
                
                current->next = temp;
                current = temp;  // current pointer'ı güncelle
            }
        }
        previous_token = ml;  // Mevcut token'ı bir sonraki iterasyon için sakla
        ml = ml->next;
    }
    
    return new_head;
}

bool is_builtin(line_t *ml)
{
    char *cmd;
    cmd = ml->value;

    return (
        ft_strncmp(cmd, "echo", 6) == 0 ||
        ft_strncmp(cmd, "cd", 6) == 0 ||
        ft_strncmp(cmd, "pwd", 6) == 0 ||
        ft_strncmp(cmd, "export", 6) == 0 ||
        ft_strncmp(cmd, "unset", 6) == 0 ||
        ft_strncmp(cmd, "env", 6) == 0 ||
        ft_strncmp(cmd, "exit", 6) == 0
    );
}

void try_builtins(line_t *ml, minishell_t *ms)
{
    char *cmd;
    line_t *filtered = filter_redirects(ml); // Yönlendirmeleri filtrele
    cmd = filtered->value;
    if(!ft_strncmp(cmd, "echo", 4))
        echo(filtered, ms);
    else if(!ft_strncmp(cmd, "cd", 6))
        cd(filtered);
    else if(!ft_strncmp(cmd, "pwd", 6))
        pwd();
    else if(!ft_strncmp(cmd, "export", 6))
        export(filtered, ms);
    else if(!ft_strncmp(cmd, "unset", 6))
        unset();
    else if(!ft_strncmp(cmd, "env", 6))
        env(ms);
    else if(!ft_strncmp(cmd, "exit", 6))
        ft_exit_2(filtered);

    free_filtered_list(filtered); // Bellek sızıntısı olmasın diye
}
