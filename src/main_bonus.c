/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 12:47:02 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/03/13 18:21:54 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

/*	called when closing window	*/
int	close_window(t_game *game)
{
	mlx_loop_end(game->ptrs.mlx);
	return (0);
}

void	print_map_on_stdout(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (y < game->scene.map.map_height)
	{
		x = 0;
		while (x < game->scene.map.map_width)
		{
			if (game->scene.map.map[y][x] == 1)
				printf(TXT_RED"%s "TXT_STD, "▄");
			else if (game->scene.map.map[y][x] == 0)
				printf(TXT_BG"%c "TXT_STD, '.');
			x++;
		}
		printf("\n");
		y++;
	}
	printf("map:	%s\nnorth:	%s\neast:	%s\nsouth	%s\nwest:	%s\n",
		game->scene.map.map_name, game->scene.textures[0].filename,
		game->scene.textures[2].filename, game->scene.textures[2].filename,
		game->scene.textures[3].filename);
	printf("sky:	0x%x\nsky2:	0x%x\nfloor:	0x%x\nfloor2:	0x%x\n",
		game->scene.ceiling_colour, game->scene.ceiling_gradient,
		game->scene.floor_colour, game->scene.floor_gradient);
}

//sets window name and inits mlx pointers
static int	initialize(t_game *game, char *mapname)
{
	char	*name;

	name = ft_strjoin("CUBE3D - ", mapname);
	if (!name)
		return (error_return(0, "init window name failed", 1));
	game->ptrs.mlx = mlx_init();
	if (game->ptrs.mlx == NULL)
	{
		free (name);
		return (error_return(0, "mlx_init failed", 2));
	}
	game->ptrs.win = mlx_new_window(game->ptrs.mlx, WINDOW_WIDTH,
			WINDOW_HEIGHT, name);
	free(name);
	if (game->ptrs.win == NULL)
		return (error_return(1, "mlx_new_window failed", 3));
	game->player.movespeed = 0.100001;
	game->player.rotspeed = 0.05;
	game->img.mlx_img = mlx_new_image(game->ptrs.mlx, WINDOW_WIDTH,
			WINDOW_HEIGHT);
	if (game->img.mlx_img == NULL)
		return (error_return(1, "image creation failed", 4));
	if (load_texture(game))
		return (error_return(1, "loading texture failed", 5));
	return (0);
}

int	main(int argc, char **argv)
{
	t_game	*game;
	int		ret;

	game = (t_game *)malloc(sizeof(t_game));
	ft_memset(game, 0, sizeof(t_game));
	ret = process_arguments(argc, argv, game);
	if (ret == 0)
	{
		ret = initialize(game, "test");
	}
	if (ret == 0)
	{
		print_map_on_stdout(game);
		printf("fog:	0x%x\n", game->scene.fog);
		mlx_loop_hook(game->ptrs.mlx, &render, game);
		mlx_hook(game->ptrs.win, PRESS, 1L << 0, &handle_keypress, game);
		mlx_hook(game->ptrs.win, DESTROY, NO_EVENT_M, &close_window, game);
		mlx_hook(game->ptrs.win, MOTION, 1L << 6, &handle_mouse, game);
		mlx_mouse_move(game->ptrs.mlx, game->ptrs.win,
			WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		mlx_loop(game->ptrs.mlx);
		mlx_do_sync(game->ptrs.mlx);
	}
	free_game(game);
	return (ret);
}
