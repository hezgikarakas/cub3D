#include "cub3D.h"

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
int		set_return_error(t_game *game, char *message)
{
	if (!game->error_message)
	{
		game->error_message = message;
	}
	return (1);
}

/* as above but with additional info (an error + the name of the file triggering the error)
   return 1 */
int		set_return_error_extra(t_game *game, char *message, char *extramessage)
{
	if (!game->error_message)
	{
		game->error_message = message;
		game->error_extramessage = extramessage;
	}
	return (1);
}

/* print error message in game and return 1 */
int			print_return_error(t_game *game)
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
