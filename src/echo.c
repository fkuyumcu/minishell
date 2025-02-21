





void	echo_flag(char **arg)
{
	int	f;
	int	i;

	f = 0;
	i = 1;
	if (arg[i] && arg[i][0] == '-')
		flag_ctrl(arg, &i);
	f = i;
	while	
}






void echo(char **arg)
{
	if (arg && arg[1])
		echo_flag(arg);
	else
		printf("\n");
}