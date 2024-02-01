/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by karakasschu       #+#    #+#             */
/*   Updated: 2024/01/25 14:17:02 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

//	suggestion of new error function, would only require 

// the subject requires the following
//   If any misconfiguration of any kind is encountered in the file the program
//   must exit properly and return "Error\n" followed by an explicit error message
//   of your choice
// My approach allows to collect error information before printing it, so it
// guarantees that the first output is "Error\n" followed by some collected detailed error message.
// I think the additional functions do not hurt, print_return is only used in main.
//
// We could use your method, without storing the message
// But let's use strerror instead of perror and just print everything directly to STDERR(2), no malloc
// I also suggest we have two functions
//  error_return_errno(message, code) for system errors
//  error_return(message, code) for all errors without a relevant errno

//1 function instead of 3, currently in use in main
int	error_return(int type, char *error_message, int error_code)
{
	char	*full_message;

	full_message = ft_strjoin("ERROR: ", error_message);
	if (type == 1)
		perror(full_message);
	else
		ft_putstr_fd(full_message, 2);
	if (full_message[sizeof(full_message)] != '\n')
		ft_putchar_fd('\n', 2);
	free(full_message);
	return (error_code);
}

static void	print_error(char *s1, char *s2)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(s1, 2);
	if (s2 != NULL)
	{
		ft_putstr_fd(s2, 2);
	}
	ft_putchar_fd('\n', 2);
}

/* set error message in game to specified message, if it is not yet set
   return 1 */
int	set_return_error(t_game *game, char *message)
{
	if (!game->error_message)
	{
		game->error_message = message;
	}
	return (1);
}

/*	as above but with additional info
	(an error + the name of the file triggering the error) return 1 */
int	set_return_error_extra(t_game *game, char *message, char *extramessage)
{
	if (!game->error_message)
	{
		game->error_message = message;
		game->error_extramessage = extramessage;
	}
	return (1);
}

/* print error message in game and return 1 */
int	print_return_error(t_game *game)
{
	if (game->error_message)
	{
		if (game->error_message)
			print_error(game->error_message, game->error_extramessage);
	}
	else
	{
		print_error("Unspecific error", NULL);
	}
	return (1);
}
