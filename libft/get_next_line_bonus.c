/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <hakaraka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 11:06:10 by hakaraka          #+#    #+#             */
/*   Updated: 2022/12/13 12:45:47 by hakaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include "libft.h"
#include <unistd.h>

char	*ft_read_rest_str(int fd, char *rest_str)
{
	char	*buf;
	int		rd_bytes;

	buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buf)
		return (NULL);
	rd_bytes = 1;
	while (!ft_strchr(rest_str, '\n') && rd_bytes != 0)
	{
		rd_bytes = read(fd, buf, BUFFER_SIZE);
		if (rd_bytes == -1)
		{
			free(buf);
			free(rest_str);
			return (NULL);
		}
		buf[rd_bytes] = '\0';
		rest_str = ft_strjoin(rest_str, buf);
	}
	free(buf);
	return (rest_str);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*rest_str[4096];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	rest_str[fd] = ft_read_rest_str(fd, rest_str[fd]);
	if (!rest_str[fd])
		return (NULL);
	line = ft_get_line(rest_str[fd]);
	rest_str[fd] = ft_new_rest_str(rest_str[fd]);
	return (line);
}
