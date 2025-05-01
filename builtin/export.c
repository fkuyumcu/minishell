/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:55:30 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/01 16:16:04 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"
#include <ctype.h>

static int	is_valid_identifier(char *str)
{
    int	i;

    if (!str || !str[0] || (!ft_isalpha(str[0]) && str[0] != '_'))
        return (0);
    i = 1;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

static char	*get_env(char *arg)
{
	char	*name;
	int		i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	name = malloc(sizeof(char) * (i + 1));
	if (!name)
		return (NULL);
	i = 0;
	while (arg[i] && arg[i] != '=')
	{
		name[i] = arg[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}

static int	find_env_index(minishell_t *ms, char *key)
{
	int	i;
	int	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (ms->envp[i])
	{
		if (ft_strncmp(ms->envp[i], key, key_len) == 0 && 
			(ms->envp[i][key_len] == '=' || ms->envp[i][key_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

static char	**alloc_new_env(minishell_t *ms, int size)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_env[i] = NULL;
		i++;
	}
	new_env[size] = NULL;
	return (new_env);
}

static void	copy_env_vars(char **dest, char **src, int size)
{
	int	i;

	i = 0;
	while (i < size && src[i])
	{
		dest[i] = strdup(src[i]);
		i++;
	}
}

static void	free_env_array(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		//free(env[i]);BAK  
		i++;
	}
	//free(env);
}

static void	update_envp(char *arg, minishell_t *ms)
{
	char	*key;
	char	**new_env;
	int		env_index;
	int		i;

	key = get_env(arg);
	if (!key)
		return ;
	env_index = find_env_index(ms, key);
	i = 0;
	while (ms->envp[i])
		i++;
	if (env_index >= 0)
	{
		free(ms->envp[env_index]);
		ms->envp[env_index] = strdup(arg);
	}
	else
	{
		new_env = alloc_new_env(ms, i + 1);
		if (!new_env)
			return ;
		copy_env_vars(new_env, ms->envp, i);
		new_env[i] = strdup(arg);
		//free_env_array(ms->envp);
		ms->envp = new_env;
	}
	free(key);
}

static void	swap_env_vars(char **env, int a, int b)
{
	char	*temp;

	temp = env[a];
	env[a] = env[b];
	env[b] = temp;
}

static void	sort_env_vars(char **sorted_env, int size)
{
    int	i;
    int	j;
    int	swapped;

    i = 0;
    while (i < size - 1)
    {
        swapped = 0;
        j = 0;
        while (j < size - i - 1)
        {
            if (((strcmp(sorted_env[j], sorted_env[j + 1])) > 0))//strcmp
            {
                swap_env_vars(sorted_env, j, j + 1);
                swapped = 1;
            }
            j++;
        }
        if (!swapped)
            break ;
        i++;
    }
}

static void	display_sorted_env_part1(minishell_t *ms, char ***sorted_env)
{
	int	i;

	i = 0;
	while (ms->envp[i])
		i++;
	*sorted_env = alloc_new_env(ms, i);
	if (!(*sorted_env))
		return ;
	copy_env_vars(*sorted_env, ms->envp, i);
	sort_env_vars(*sorted_env, i);
}

static void	display_sorted_env_part2(char **sorted_env)
{
	int	i;

	i = 0;
	while (sorted_env[i])
	{
		printf("declare -x %s\n", sorted_env[i]);
		i++;
	}
	//free_env_array(sorted_env);
}

static void	display_sorted_env(minishell_t *ms)
{
	char	**sorted_env;

	display_sorted_env_part1(ms, &sorted_env);
	if (!sorted_env)
		return ;
	display_sorted_env_part2(sorted_env);
}

void	export(line_t *ml, minishell_t *ms)
{
	line_t	*current;

	current = ml->next;
	if (!current)
	{
		display_sorted_env(ms);
		return ;
	}
	while (current)
	{
		if (!is_valid_identifier(current->value))
			printf("minishell: export: `%s': not a valid identifier\n",
				current->value);
		else
			update_envp(current->value, ms);
		current = current->next;
	}
}