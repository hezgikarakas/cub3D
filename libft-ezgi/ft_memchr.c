/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 11:07:53 by hakaraka          #+#    #+#             */
/*   Updated: 2022/10/17 11:12:36 by hakaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	*str_end;

	str = (unsigned char *)s;
	str_end = (unsigned char *)(s + n);
	while (str != str_end)
	{
		if (*str == (unsigned char)c)
		{
			return ((void *)str);
		}
		str++;
	}
	return (0);
}
