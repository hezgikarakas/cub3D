/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 16:05:23 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/02/08 16:43:42 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

//	gets textures from file
static void	convert_texture(t_game *game, t_texture *tex)
{
	tex->texture_img = mlx_xpm_file_to_image(game->ptrs.mlx, tex->filename,
			&tex->width, &tex->height);
	tex->texture_addr = mlx_get_data_addr(tex->texture_img, &tex->bpp,
			&tex->line_len, &tex->endian);
}
void	load_texture2(t_game *game)
{
	convert_texture(game, &game->scene.textures[0]);
	convert_texture(game, &game->scene.textures[1]);
	convert_texture(game, &game->scene.textures[2]);
	convert_texture(game, &game->scene.textures[3]);
}

void	get_default_texture(int dir, t_game *game)
{
	printf("couldn't find texture [%i] using default texture\n", dir);
	game->scene.textures[dir].filename = 0;
	if (dir == 0
		&& open("./textures/lionwall.xpm", O_RDONLY, __O_CLOEXEC) != -1)
			game->scene.textures[dir].filename = "./textures/lionwall.xpm";
	if (dir == 1
		&& open("./textures/patternwall.xpm", O_RDONLY, __O_CLOEXEC) != -1)
			game->scene.textures[dir].filename = "./textures/patternwall.xpm";
	if (dir == 2
		&& open("./textures/vinewall.xpm", O_RDONLY, __O_CLOEXEC) != -1)
			game->scene.textures[dir].filename = "./textures/vinewall.xpm";
	if (dir == 3
		&& open("./textures/pillarwall.xpm", O_RDONLY, __O_CLOEXEC) != -1)
			game->scene.textures[dir].filename = "./textures/pillarwall.xpm";
}

void	load_texture(t_game *game, int dir)
{
	if (!game->scene.textures[dir].filename
		|| open(game->scene.textures[dir].filename, O_RDONLY, __O_CLOEXEC) == -1)
		get_default_texture(dir, game);
	// convert_texture(game, &game->scene.textures[dir]);
}
