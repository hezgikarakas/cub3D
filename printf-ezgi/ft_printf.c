/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 08:38:19 by hakaraka          #+#    #+#             */
/*   Updated: 2022/12/06 08:38:52 by hakaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_printf_checklist(char s, va_list *args, int *p_len, int *i)
{
	if (s == 's')
		ft_putstr(va_arg(*args, char *), p_len);
	else if (s == 'd' || s == 'i')
		ft_number(va_arg(*args, int), p_len);
	else if (s == 'u')
		ft_unsigned_int(va_arg(*args, unsigned int), p_len);
	else if (s == 'x')
		ft_hexadecimal(va_arg(*args, unsigned int), p_len, 'x');
	else if (s == 'X')
		ft_hexadecimal(va_arg(*args, unsigned int), p_len, 'X');
	else if (s == 'p')
		ft_pointer(va_arg(*args, size_t), p_len);
	else if (s == 'c')
		ft_putchar(va_arg(*args, int), p_len);
	else if (s == '%')
		ft_putchar('%', p_len);
	else
		(*i)--;
}

int	ft_printf(const char *input, ...)
{
	va_list	args;
	int		i;
	int		length;

	i = 0;
	length = 0;
	va_start(args, input);
	while (input[i] != '\0')
	{
		if (input[i] == '%')
		{
			i++;
			ft_printf_checklist(input[i], &args, &length, &i);
			i++;
		}
		else
		{
			ft_putchar(input[i], &length);
			i++;
		}
	}
	va_end(args);
	return (length);
}
