/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <hakaraka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:54:31 by hakaraka          #+#    #+#             */
/*   Updated: 2022/10/31 09:48:57 by hakaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = ft_strlen(needle);
	if (i == 0)
		return ((char *)haystack);
	k = i;
	i = 0;
	while (haystack[i] && i < len)
	{
		if (haystack[i] == needle[0])
		{
			j = -1;
			k = ft_strlen(needle);
			while (needle[++j] && needle[j] == haystack[i + j] && i + j < len)
				k--;
			if (k == 0)
				return ((char *)haystack + i);
		}
		i++;
	}
	return (0);
}
