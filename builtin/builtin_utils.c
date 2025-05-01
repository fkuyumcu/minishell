/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:05:44 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/01 16:11:34 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

void free_filtered(line_t *head)
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

static bool is_redirect_type(int type)
{
    return (type == REDIRECT_IN || type == REDIRECT_OUT
        || type == HEREDOC_IN || type == HEREDOC_OUT);
}

static line_t *copy_line(line_t *src, minishell_t *ms)
{
    line_t *new;

    new = malloc(sizeof(line_t));
    if (!new)
        return (NULL);
    new->value = ft_strdup(src->value, ms);
    new->type = src->type;
    new->next = NULL;
    new->priority = src->priority;
    return (new);
}

static bool should_add_arg(line_t *ml, line_t *prev)
{
    if (!is_redirect_type(ml->type)
        && !(prev && is_redirect_type(prev->type)))
        return (true);
    return (false);
}

static void filter_args_loop(line_t **ml, line_t **prev,
        line_t **current, minishell_t *ms)
{
    line_t *temp;

    while (*ml)
    {
        if (should_add_arg(*ml, *prev))
        {
            temp = copy_line(*ml, ms);
            if (!temp)
                return ;
            (*current)->next = temp;
            *current = temp;
        }
        *prev = *ml;
        *ml = (*ml)->next;
    }
}

line_t *filter_args(line_t *ml, minishell_t *ms)
{
    line_t *new_head;
    line_t *current;
    line_t *temp;
    line_t *prev;

    if (!ml)
        return (NULL);
    temp = copy_line(ml, ms);
    if (!temp)
        return (NULL);
    new_head = temp;
    current = temp;
    prev = ml;
    ml = ml->next;
    filter_args_loop(&ml, &prev, &current, ms);
    return (new_head);
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
    line_t *filtered = filter_args(ml, ms);
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

    free_filtered(filtered);
}
