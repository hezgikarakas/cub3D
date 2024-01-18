/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printwords.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 08:39:48 by hakaraka          #+#    #+#             */
/*   Updated: 2022/12/06 08:39:51 by hakaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

//characters
void	ft_putchar(char c, int *p_len)
{
	write(1, &c, 1);
	(*p_len)++;
}

//string
void	ft_putstr(char *arg, int *p_len)
{
	size_t	i;

	i = 0;
	if (!arg)
	{
		write(1, "(null)", 6);
		(*p_len) += 6;
		return ;
	}
	while (arg[i] != '\0')
	{
		ft_putchar(arg[i], p_len);
		i++;
	}
}
