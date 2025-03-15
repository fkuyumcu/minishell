/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:22:20 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/15 12:57:27 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"






char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	n;
	size_t	i;

	n = ft_strlen(needle);
	i = 0;
	if (! *needle)
		return ((char *)haystack);
	while (haystack[i] && i + n <= len)
	{
		if (haystack[i] == *needle && ft_strncmp(&haystack[i], needle, n) == 0)
			return ((char *)&haystack[i]);
		i++;
	}
	return (NULL);
}


int	count_tokens(char const *s, char c)
{
	char	*q;
	int		count;

	q = (char *)s;
	count = 0;
	while (*q)
	{
		if (*q == c)
			q++;
		else
		{
			count++;
			while (*q && *q != c)
				q++;
		}
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	char		**str_arr;
	int			i;
	int			j;
	int			count;

	if (!s)
		return (0);
	count = count_tokens(s, c);
	str_arr = malloc(sizeof(char *) * (count + 1));
	if (!str_arr)
		return (0);
	i = -1;
	j = 0;
	while (++i < count)
	{
		while (*s == c)
			s++;
		j = 0;
		while (*(s + j) != c && *(s + j))
			j++;
		*(str_arr + i) = ft_substr(s, 0, j);
		s += j;
	}
	*(str_arr + i) = 0;
	return (str_arr);
}



void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		++s;
	}


}



char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	size;
	const char		*firstc;
	char			*address;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (strdup(""));//strdup
	size = ft_strlen(s + start);
	if (size < len)
		len = size;
	firstc = &s[start];
	address = (char *)malloc(len + 1);
	if (address == NULL)
		return (NULL);
	ft_memcpy(address, firstc, len);
	address[len] = '\0';
	return (address);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*p;
	unsigned char	*q;

	if (!dest && !src)
		return (NULL);
	p = (unsigned char *) src;
	q = (unsigned char *) dest;
	while (n--)
	{
		q[n] = p[n];
	}
	return (dest);
}

