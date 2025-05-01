#include <unistd.h>
#include "../src/minishell.h"

int	ft_isdigit_str(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}
int	ft_atoi(char *str)
{
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 1;
	while (str[i] == 32 || (str[i] <= 13 && str[i] >= 9))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			k = -1;
		i++;
	}
	while ((str[i] <= 57 && str[i] >= 48))
	{
		j = j * 10 + (str[i] - 48);
		i++;
	}
	return (k * j);
}

void ft_exit_2(line_t *ml)
{
	int status;

	if (ft_isdigit_str(ml->next->value) != 0)
	{
		write(1, "exit\n", 5);
		write(2, "numeric argument requierd ", 26);
		exit(2);
	}
	if (ml->next->next != NULL)
	{
		write(1, "exit\n", 5);
		write(1, "minishell: exit: too many arguments\n", 36);
		return;
	}
	if (ml->value == NULL)
	{
		write(1, "exit\n", 5);
		exit(0);
	}
	status = ft_atoi(ml->next->value);
	if (status < 0)
		status = 256 + status;
	else if (status > 255)
		status = status % 256;
	write(1, "exit\n", 5);
	global_code = status;
	exit(global_code);
}
