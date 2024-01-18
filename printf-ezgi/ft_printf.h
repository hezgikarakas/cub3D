/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 08:39:23 by hakaraka          #+#    #+#             */
/*   Updated: 2022/12/06 08:39:26 by hakaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

void	ft_putchar(char c, int *p_len);
void	ft_putstr(char *arg, int *p_len);
void	ft_number(int number, int *p_len);
void	ft_hexadecimal(unsigned int x, int *p_len, char x_or_x);
void	ft_unsigned_int(unsigned int u, int *p_len);
void	ft_pointer(size_t pointer, int *p_len);
int		ft_printf(const char *input, ...);

#endif
