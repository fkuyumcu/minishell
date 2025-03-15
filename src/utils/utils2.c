/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:22:20 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/03/04 15:04:14 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
char	*ft_strjoin(char *s1, char *s2, minishell_t *minishell)
{
	int		a;
	int		b;
	int		c;
	char	*str;

	a = ft_strlen(s1);
	b = ft_strlen(s2);
	c = 0;
	str = malloc((a + b) * sizeof(char) + 1);
	if (!str)
		ft_error(minishell, "Error Allocating Space");
	while (c < a)
	{
		str[c] = s1[c];
		c++;
	}
	c = 0;
	while (c < b)
	{
		str[a + c] = s2[c];
		c++;
	}
	str[a + c] = '\0';
	free(s1);
	return (str);
}



char	*ft_strdup(const char *s1, minishell_t *minishell)
{
	char	*adress;

	adress = malloc(ft_strlen(s1)+1);
	if (adress == NULL)
		ft_error(minishell, "Error Allocating Space");
	ft_memcpy(adress, s1, ft_strlen(s1) + 1);
	return (adress);
}


char	*ft_strchr(const char *s, int c)
{
	size_t	len;
	size_t	i;
	char	*x;

	len = ft_strlen(s);
	i = 0;
	while (i <= len)
	{
		if (s[i] == (char)c)
		{
			x = (char *) &s[i];
			return (x);
		}
		i++;
	}
	return (NULL);
}

