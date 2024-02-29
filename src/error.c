/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <hakaraka@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/02/29 13:17:53 by hakaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

int	error_return(int type, char *error_message, int error_code)
{
	char	*full_message;

	full_message = ft_strjoin("Error\n", error_message);
	if (type == 1)
		perror(full_message);
	else
		ft_putstr_fd(full_message, 2);
	if (full_message[sizeof(full_message)] != '\n')
		ft_putchar_fd('\n', 2);
	free(full_message);
	return (error_code);
}

int	error_return_s(int type, char *error_message, int error_code, char *extra)
{
	char	*full_message;
	int		ret;

	full_message = ft_strjoin(error_message, extra);
	ret = error_return(type, full_message, error_code);
	free(full_message);
	return (ret);
}
