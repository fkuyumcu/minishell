#include "minishell.h"

void	cpy_enviroment(char **env, minishell_t *minishell)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	minishell->env = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		minishell->env[i] = ft_strdup(env[i]);
		i++;
	}
	minishell->env[i] = NULL;
	
}