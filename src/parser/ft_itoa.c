#include "../minishell.h"

static int	intlen(long c)
{
	int	count;

	count = 1;
	while (c / 10 > 0)
	{
		c /= 10;
		count++;
	}
	return (count);
}

static void	set(char *s, long c, int len, int sign)
{
	int	tmp;

	tmp = len;
	if (sign == 1)
		s[0] = '-';
	while (len > 0)
	{
		s[len + sign - 1] = (c % 10) + 48;
		c = c / 10;
		len--;
	}
	s[tmp + sign] = '\0';
}

char	*ft_itoa(int n)
{
	int		sign;
	char	*s;
	int		len;
	long	a;

	sign = 0;
	a = n;
	if (n < 0)
	{
		sign = 1;
		a = -a;
	}
	len = intlen(a);
	s = malloc((len + sign + 1) * sizeof(char));
	if (!s)
		return (0);
	set(s, a, len, sign);
	return (s);
}