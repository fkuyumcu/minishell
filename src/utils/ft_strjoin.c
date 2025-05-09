/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:22:20 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/05/09 09:02:20 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin(char *s1, char *s2, t_minishell *minishell)
{
	size_t	c;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	c = 0;
	str = NULL;
	str = malloc((ft_strlen(s1) + ft_strlen(s2)) * sizeof(char) + 1);
	if (!str)
		ft_error(minishell, "Error Allocating Space");
	while (c < ft_strlen(s1))
	{
		str[c] = s1[c];
		c++;
	}
	c = 0;
	while (c < ft_strlen(s2))
	{
		str[ft_strlen(s1) + c] = s2[c];
		c++;
	}
	str[ft_strlen(s1) + c] = '\0';
	return (str);
}

char	*ft_strdup(const char *s1, t_minishell *minishell)
{
	char	*adress;

	adress = NULL;
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
