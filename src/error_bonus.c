/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/04/04 17:36:14 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

int	error_return(int type, char *error_message, int error_code)
{
	ft_putstr_fd("ERROR: ", 2);
	if (type == 1)
		perror(error_message);
	else
	{
		ft_putstr_fd(error_message, 2);
		if (error_message[sizeof(error_message)] != '\n')
			ft_putchar_fd('\n', 2);
	}
	return (error_code);
}

int	error_return_s(int type, char *error_message, int error_code, char *extra)
{
	ft_putstr_fd("ERROR: ", 2);
	if (type == 1)
		perror(error_message);
	else
	{
		ft_putstr_fd(error_message, 2);
		if (error_message[sizeof(error_message)] != '\n')
			ft_putchar_fd('\n', 2);
	}
	ft_putstr_fd(extra, 2);
	if (extra[ft_strlen(extra) - 1] != '\n')
		ft_putchar_fd('\n', 2);
	return (error_code);
}
