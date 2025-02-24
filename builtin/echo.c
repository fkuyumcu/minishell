int	is_flag_n(char *arg)
{
	int	i;

	i = 1;
	if (arg[0] != '-' || arg[1] != 'n')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	flag_ctrl(char **arg, int *i)
{
	if (is_flag_n(arg[*i]) == 1)
		*i += 1;
}

void	echo_with_arg(char **arg)
{
	int	flag;

	flag = 0;
	i = 1;
	if (arg[i] && arg[i][0] == '-')
		flag_ctrl(arg, &i);
	while (arg[i])
	{
		printf("%s", arg[i]);
		if (arg[i + 1])
			printf(" ");
		i++;
	}
	if (is_flag_n(arg[1]) == 0)
		printf("\n");
}

void	echo(char **arg)
{
	if (arg && arg[1])
		echo_with_arg(arg);
	else
		printf("\n");
}
