/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/04/04 17:36:14 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

void	free_strings(char **s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i])
			free(s[i]);
		i++;
	}
	if (s)
		free(s);
}

static void	free_map(t_map *map)
{
	int	i;

	i = 0;
	while (i < map->map_height)
	{
		free(map->map[i]);
		++i;
	}
	free(map->map);
}

static void	free_textures(void *mlx_ptr, t_texture *textures)
{
	int	i;

	i = 0;
	while (i < NUMBER_OF_TEXTURES)
	{
		if (textures[i].filename)
			free(textures[i].filename);
		if (textures[i].texture_img)
			mlx_destroy_image(mlx_ptr, textures[i].texture_img);
		i++;
	}
}

/* free everything that is not NULL and was dynamically allocated
 * - texture names
 * - map
 * - mlx */
void	free_game(t_game *game)
{
	free_textures(game->ptrs.mlx, game->scene.textures);
	if (game->scene.map.map)
		free_map(&game->scene.map);
	if (game->img.mlx_img)
		mlx_destroy_image(game->ptrs.mlx, game->img.mlx_img);
	if (game->ptrs.win)
		mlx_destroy_window(game->ptrs.mlx, game->ptrs.win);
	if (game->ptrs.mlx)
	{
		mlx_destroy_display(game->ptrs.mlx);
		free(game->ptrs.mlx);
	}
}

void	setmaxlinelen(t_parse_helper *ph, char *line_temp)
{
	if (!ph->found_map_start && !ph->interpreted_this_line)
	{
		ph->found_map_start = 1;
		ph->map_start_idx = ph->line_idx;
	}
	if (ph->found_map_start && !ph->found_map_end)
	{
		ph->linelength = ft_strlen(line_temp);
		if (line_temp[ph->linelength - 1] == '\n')
			ph->linelength--;
		if (ph->linelength > ph->map_max_line_length)
			ph->map_max_line_length = ph->linelength;
	}
}
