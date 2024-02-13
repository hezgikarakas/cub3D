/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 12:47:02 by karakasschu       #+#    #+#             */
/*   Updated: 2024/02/13 17:03:12 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

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
				printf("\033[1;31;40m%i \033[0m", game->scene.map.map[y][x]);
			else
				printf("\033[40m%i \033[0m", game->scene.map.map[y][x]);
			x++;
		}
		printf("\n");
		y++;
	}
}

//sets window name and inits mlx pointers
static int	initialize(t_game *game, char *mapname)
{
	char	*name;

	print_map_on_stdout(game);
	name = ft_strjoin("CUBE3D - ", mapname);
	game->ptrs.mlx = mlx_init();
	if (game->ptrs.mlx == NULL)
		return (error_return(0, "mlx_init failed", 2));
	game->ptrs.win = mlx_new_window(game->ptrs.mlx, WINDOW_WIDTH,
			WINDOW_HEIGHT, name);
	free(name);
	if (game->ptrs.win == NULL)
	{
		free(game->ptrs.mlx);
		return (error_return(1, "mlx_new_window failed", 3));
	}
	game->player.movespeed = 0.100001;
	game->player.rotspeed = 0.05;
	// game->player.pos_y += 0.5;
	// game->player.pos_x += 0.5;
	game->img.mlx_img = mlx_new_image(game->ptrs.mlx, WINDOW_WIDTH,
			WINDOW_HEIGHT);
	load_texture(game);
	return (0);
}

int	main(int argc, char **argv)
{
	t_game	*game;
	int		ret;

	game = (t_game *)malloc(sizeof(t_game));
	ft_memset(game, 0, sizeof(t_game));
	if (!game)
		return (error_return(1, "game struct malloc failed", 1));
	ret = process_arguments(argc, argv, game);
	if (ret == 0)
		ret = initialize(game, "test");
	if (ret == 0)
	{
		printf("%s\n", game->scene.map.map_name);
		printf("%s\n", game->scene.textures[0].filename);
		printf("%s\n", game->scene.textures[1].filename);
		printf("%s\n", game->scene.textures[2].filename);
		printf("%s\n", game->scene.textures[3].filename);
		printf("%x\n", game->scene.ceiling_colour);
		printf("%x\n", game->scene.floor_colour);
		printf("%x\n", DEFAULT_FLOOR);
		mlx_loop_hook(game->ptrs.mlx, &render, game);
		mlx_hook(game->ptrs.win, 2, 1L << 0, &handle_keypress, game);
		mlx_hook(game->ptrs.win, 17, 0L, &close_window, game);
		mlx_loop(game->ptrs.mlx);
	}
	free_game(game);
	return (ret);
}
