/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:00:47 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/04/29 16:01:41 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_strlcat(char *dst, const char *src, size_t n)
{
	size_t	dstsize;
	size_t	i;
	size_t	srcsize;

	srcsize = ft_strlen(src);
	dstsize = ft_strlen(dst);
	i = 0;
	if (n <= dstsize)
		return (n + srcsize);
	while (src[i] && (dstsize + i + 1) < n)
	{
		dst[dstsize + i] = src[i];
		i++;
	}
	dst[dstsize + i] = '\0';
	return (dstsize + srcsize);
}