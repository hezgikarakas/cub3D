/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <hakaraka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 12:50:34 by hakaraka          #+#    #+#             */
/*   Updated: 2022/10/31 10:28:34 by hakaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t				i;
	const unsigned char	*c1;
	const unsigned char	*c2;

	i = 0;
	c1 = (const unsigned char *)s1;
	c2 = (const unsigned char *)s2;
	while ((c1[i] != '\0' || c2[i] != '\0') && i < n)
	{
		if (c1[i] != c2[i])
		{
			return ((int)c1[i] - (int)c2[i]);
		}
		i++;
	}
	return (0);
}
