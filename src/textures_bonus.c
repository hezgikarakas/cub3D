/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 16:05:23 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/03/11 20:16:36 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

//	gets textures from file
static int	convert_texture(t_game *game, t_texture *tex)
{
	tex->texture_img = mlx_xpm_file_to_image(game->ptrs.mlx, tex->filename,
			&tex->width, &tex->height);
	if (tex->texture_img == NULL)
		return (1);
	tex->texture_addr = mlx_get_data_addr(tex->texture_img, &tex->bpp,
			&tex->line_len, &tex->endian);
	if (tex->texture_addr == NULL)
		return (1);
	return (0);
}

int	load_texture(t_game *game)
{
	if (convert_texture(game, &game->scene.textures[0]))
		return (1);
	if (convert_texture(game, &game->scene.textures[1]))
		return (1);
	if (convert_texture(game, &game->scene.textures[2]))
		return (1);
	if (convert_texture(game, &game->scene.textures[3]))
		return (1);
	return (0);
}
