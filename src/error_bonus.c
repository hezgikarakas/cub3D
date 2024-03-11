/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/03/11 20:16:55 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

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
	if (extra[sizeof(extra)] != '\n')
		ft_putchar_fd('\n', 2);
	return (error_code);
}
