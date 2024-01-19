/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <hakaraka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:54:52 by hakaraka          #+#    #+#             */
/*   Updated: 2022/10/31 10:28:08 by hakaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		len;
	int		i;
	char	*buffer;

	len = 0;
	i = 0;
	while (s1[len])
		len++;
	buffer = (char *)malloc(sizeof(*buffer) * (len + 1));
	if (!buffer)
		return (0);
	while (i < len)
	{
		buffer[i] = s1[i];
		i++;
	}
	buffer[i] = 0;
	return (buffer);
}
