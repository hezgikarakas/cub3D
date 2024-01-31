/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 16:05:23 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/01/31 15:50:27 by jkatzenb         ###   ########.fr       */
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

void	load_texture(t_game *game)
{
	convert_texture(game, &game->scene.textures[0]);
	convert_texture(game, &game->scene.textures[1]);
	convert_texture(game, &game->scene.textures[2]);
	convert_texture(game, &game->scene.textures[3]);
}
