/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 12:47:02 by karakasschu       #+#    #+#             */
/*   Updated: 2024/01/31 16:25:44 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

//sets window name and inits mlx pointers
static int	initialize(t_game *game, char *mapname)
{
	char	*name;

	game->rc = (t_rc *)malloc(sizeof(t_rc));
	if (!game->rc)
		error_return(1, "rc struct malloc failed", 1);
	name = ft_strjoin("CUBE3D - ", mapname);
	game->ptrs.mlx = mlx_init();
	if (game->ptrs.mlx == NULL)
		error_return(1, "mlx_init failed", 2);
	game->ptrs.win = mlx_new_window(game->ptrs.mlx, WINDOW_WIDTH,
			WINDOW_HEIGHT, name);
	free(name);
	if (game->ptrs.win == NULL)
	{
		free(game->ptrs.mlx);
		error_return(1, "mlx_new_window failed", 3);
	}
	game->player.plane_x = 0;
	game->player.plane_y = 0.66;
	game->player.movespeed = 0.1;
	game->player.rotspeed = 0.05;
	game->img.mlx_img = mlx_new_image(game->ptrs.mlx, WINDOW_WIDTH,
		WINDOW_HEIGHT);
	return (0);
}
// function to be removed once input parser is fully functional and connected to renderer
static void	temp_interpreter_bypass(int ac, char **av, t_game *game)
{
	if (ac || av || !ac)
	{
		game->player.pos_x = 3;
		game->player.pos_y = 5;
		game->player.look_x = -1;
		game->player.look_y = 0;
		game->scene.colors[0][0] = DEFAULT_FLOOR;
		game->scene.colors[1][0] = DEFAULT_SKY;
		game->scene.textures[0].filename = "./textures/lionwall.xpm";
		game->scene.textures[1].filename = "./textures/patternwall.xpm";
		game->scene.textures[2].filename = "./textures/vinewall.xpm";
		game->scene.textures[3].filename = "./textures/pillarwall.xpm";
		load_texture(game);
		game->scene.map.map_height = 20;
		game->scene.map.map_width = 20;
		game->scene.map.map = allocate_map(game->scene.map.map_height, game->scene.map.map_width);
		for (int x = 0; x < game->scene.map.map_height; x++){
			for (int y = 0; y < game->scene.map.map_width; y++){
				int tile = 0;
				if (x == 0 || x == game->scene.map.map_height-1
					|| y == 0 || y == game->scene.map.map_width-1)
					tile = 1;
				game->scene.map.map[x][y] = tile;
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
		for (int i = 0; i < game->scene.map.map_height; i++){
			for (int j = 0; j < game->scene.map.map_width; j++){
				if (game->scene.map.map[i][j] == 1)
					printf("\033[1;31;40m%i \033[0m", game->scene.map.map[i][j]);
				else
					printf("\033[40m%i \033[0m", game->scene.map.map[i][j]);
			}
			printf("\n");
		}
	}
}

int	main(int argc, char **argv)
{
	t_game	*game;
	int		ret;

	game = (t_game *)malloc(sizeof(t_game));
	if (!game)
		return (error_return(1, "game struct malloc failed", 1));
	// ret = validate_arguments(argc, argv, game);
	ret = 0;
	if (!ret)
		ret = initialize(game, "test");
	if (!ret)
	{
		//	temp interpreter bypass gets replaced by validate arguments
		//	some things in bypass might need to be moved to initialize
		temp_interpreter_bypass(argc, argv, game);
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
