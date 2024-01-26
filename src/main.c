/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 12:47:02 by karakasschu       #+#    #+#             */
/*   Updated: 2024/01/25 16:44:23 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

//sets window name and inits mlx pointers
void	initialize(t_game *game, char *mapname)
{
	//need to change exit to return here to avoid leaks
	char	*name;

	game->rc = (t_rc *)malloc(sizeof(t_rc));
	ft_memset(game->rc, 0, sizeof(t_rc)); // otherwise valgrind will complain a lot
	if (!game->rc)
		exit(1);
	name = ft_strjoin("CUBE3D - ", mapname);
	game->ptrs.mlx = mlx_init();
	if (game->ptrs.mlx == NULL)
	{
		perror("mlx init failed");
		exit(2);
	}
	game->ptrs.win = mlx_new_window(game->ptrs.mlx, WINDOW_WIDTH,
			WINDOW_HEIGHT, name);
	free(name);
	if (game->ptrs.win == NULL)
	{
		free(game->ptrs.mlx);
		perror("window creation failed");
		exit(3);
	}
	game->player.plane_x = 0;
	game->player.plane_y = 0.66;
	game->player.movespeed = 0.1;
	game->player.rotspeed = 0.05;
}
// function to be removed once input parser is fully functional and connected to renderer
// not static or we cannot comment it out below ;)
int	temp_interpreter_bypass(int ac, char **av, t_game *game)
{
	if (ac || av || !ac)
	{
		game->player.pos_x = 3;
		game->player.pos_y = 5;
		game->player.look_x = -1;
		game->player.look_y = 0;
		game->scene.colors[0][0] = DEFAULT_FLOOR;
		game->scene.colors[1][0] = DEFAULT_SKY;
		game->scene.map.map_height = 20;
		game->scene.map.map_width = 20;
		game->scene.map.map = allocate_map(game->scene.map.map_height, game->scene.map.map_width);
		for (int x = 0; x < game->scene.map.map_height; x++){
			for (int y = 0; y < game->scene.map.map_width; y++){
				int tile = 0;
				if (x == 0 || x == game->scene.map.map_height-1
					|| y == 0 || y == game->scene.map.map_width-1)
					tile = 1;
				// you initialized map[x][y] but it should be map[y][x] which is more logical (line-based reading)
				game->scene.map.map[y][x] = tile;
			}
		}
		game->scene.map.map[2][1] = 1;
		game->scene.map.map[2][2] = 1;
		game->scene.map.map[1][2] = 1;
		game->scene.map.map[14][1] = 1;
		game->scene.map.map[14][2] = 1;
		game->scene.map.map[14][4] = 1;
		game->scene.map.map[14][5] = 1;
		game->scene.map.map[14][6] = 1;
		game->scene.map.map[15][6] = 1;
		game->scene.map.map[16][6] = 1;
		game->scene.map.map[17][6] = 1;
		game->scene.map.map[18][6] = 1;
		game->scene.map.map[5][13] = 1;
		game->scene.map.map[5][14] = 1;
		game->scene.map.map[6][13] = 1;
		game->scene.map.map[6][14] = 1;
		game->scene.map.map[13][16] = 1;
		game->scene.map.map[13][13] = 1;
		game->scene.map.map[16][13] = 1;
		game->scene.map.map[16][16] = 1;
	}
	return (0); // always successful
}

void print_map_on_stdout(t_game* game)
{
	// here it is correctly done: map[y][x] (first the lines, then the columns)
	// (i replaced i by y and j by x)
	for (int y = 0; y < game->scene.map.map_height; y++){
		for (int x = 0; x < game->scene.map.map_width; x++){
			if (game->scene.map.map[y][x] == 1)
				printf("\033[1;31;40m%i \033[0m", game->scene.map.map[y][x]);
			else
				printf("\033[40m%i \033[0m", game->scene.map.map[y][x]);
		}
		printf("\n");
	}
}

int	main(int argc, char **argv)
{
	t_game	*game;
	int		ret;

	game = (t_game *)malloc(sizeof(t_game));
	ft_memset(game, 0, sizeof(t_game)); // otherwise valgrind will complain a lot
	if (!game)
		return (error_return(1, "game struct malloc failed", 1));
	ret = process_arguments(argc, argv, game);
//	ret = temp_interpreter_bypass(argc, argv, game);

	if (ret == 0)
	{
		print_map_on_stdout(game);
		initialize(game, "test");
		mlx_loop_hook(game->ptrs.mlx, &render, game);
		mlx_hook(game->ptrs.win, 2, 1L << 0, &handle_keypress, game);
		mlx_hook(game->ptrs.win, 17, 0L, &close_window, game);
		mlx_loop(game->ptrs.mlx);
		mlx_destroy_window(game->ptrs.mlx, game->ptrs.win);
		mlx_destroy_display(game->ptrs.mlx);
	}
	free(game->ptrs.mlx);
	free(game->rc);
	free(game);
	return (ret);
}
