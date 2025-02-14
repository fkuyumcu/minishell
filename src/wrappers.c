/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkuyumcu <fkuyumcu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:22:20 by fkuyumcu          #+#    #+#             */
/*   Updated: 2025/02/14 16:59:04 by fkuyumcu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_getcwd(char *buf, size_t size)
{
    if(getcwd(buf, size) == NULL)
        perror(RED"getcwd failed\n"RST);
        
}
