/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 12:47:02 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/03/11 19:30:07 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

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
	printf("map:	%s\n", game->scene.map.map_name);
	printf("north:	%s\n", game->scene.textures[0].filename);
	printf("east:	%s\n", game->scene.textures[1].filename);
	printf("south	%s\n", game->scene.textures[2].filename);
	printf("west:	%s\n", game->scene.textures[3].filename);
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
	load_texture(game);
	return (0);
}

int	main(int argc, char **argv)
{
	static t_game	*game;
	int				ret;

	ret = process_arguments(argc, argv, game);
	if (ret == 0)
	{
		ret = initialize(game, "test");
		print_map_on_stdout(game);
		printf("sky:	0x%x\n", game->scene.ceiling_colour);
		printf("sky2:	0x%x\n", game->scene.ceiling_gradient);
		printf("floor:	0x%x\n", game->scene.floor_colour);
		printf("floor2:	0x%x\n", game->scene.floor_gradient);
		printf("fog:	0x%x\n", game->scene.fog);
	}
	if (ret == 0)
	{
		mlx_loop_hook(game->ptrs.mlx, &render, game);
		mlx_hook(game->ptrs.win, 2, 1L << 0, &handle_keypress, game);
		mlx_hook(game->ptrs.win, 17, 0L, &close_window, game);
		mlx_loop(game->ptrs.mlx);
	}
	free_game(game);
	return (ret);
}
