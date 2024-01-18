/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnumbers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 08:39:36 by hakaraka          #+#    #+#             */
/*   Updated: 2022/12/06 08:39:38 by hakaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

//hexadecimal (x or X)
void	ft_hexadecimal(unsigned int x, int *p_len, char x_or_x)
{
	char	string[25];
	char	*base_char;
	int		i;

	if (x_or_x == 'X')
		base_char = "0123456789ABCDEF";
	else
		base_char = "0123456789abcdef";
	i = 0;
	if (x == 0)
	{
		ft_putchar('0', p_len);
		return ;
	}
	while (x != 0)
	{
		string[i] = base_char[x % 16];
		x = x / 16;
		i++;
	}
	while (i > 0)
	{
		i--;
		ft_putchar(string[i], p_len);
	}
}

//pointer
void	ft_pointer(size_t pointer, int *p_len)
{
	char	string[25];
	char	*base_char;
	int		i;

	base_char = "0123456789abcdef";
	i = 0;
	if (pointer == 0)
	{
		write(1, "(nil)", 5);
		(*p_len) += 5;
		return ;
	}
	write(1, "0x", 2);
	(*p_len) += 2;
	while (pointer != 0)
	{
		string[i] = base_char[pointer % 16];
		pointer = pointer / 16;
		i++;
	}
	while (i > 0)
	{
		i--;
		ft_putchar(string[i], p_len);
	}
}

//unsigned int
void	ft_unsigned_int(unsigned int u, int *p_len)
{
	if (u >= 10)
		ft_unsigned_int(u / 10, p_len);
	ft_putchar(u % 10 + '0', p_len);
}

//numbers
void	ft_number(int number, int *p_len)
{
	if (number == -2147483648)
	{
		write(1, "-2147483648", 11);
		(*p_len) += 11;
		return ;
	}
	if (number < 0)
	{
		ft_putchar('-', p_len);
		ft_number(number * -1, p_len);
	}
	else
	{
		if (number > 9)
			ft_number(number / 10, p_len);
		ft_putchar(number % 10 + '0', p_len);
	}
}
