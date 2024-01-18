/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <hakaraka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 11:06:31 by hakaraka          #+#    #+#             */
/*   Updated: 2022/12/13 11:13:32 by hakaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif

char	*get_next_line(int fd);
char	*ft_read_rest_str(int fd, char *rest_str);
char	*ft_strchr(char *s, int c);
char	*ft_strjoin(char *rest_str, char *buf);
size_t	ft_strlen(char *s);
char	*ft_get_line(char *rest_str);
char	*ft_new_rest_str(char *rest_str);

#endif
