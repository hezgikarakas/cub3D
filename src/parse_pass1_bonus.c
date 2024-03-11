/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pass1_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by jkatzenb          #+#    #+#             */
/*   Updated: 2024/03/11 18:31:18 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	handle_potential_map_end(t_parse_helper *ph)
{
	if (ph->found_map_start && !ph->found_map_end)
	{
		ph->found_map_end = 1;
		ph->map_end_idx = ph->line_idx;
	}
}

/* check if line is only spaces
     if yes and found_map_start then found_map_end = 1, record map end idx
     if yes and not found_map_start then skip
     if not and found_map_end then error
   check if we are not in the map and the line is longer than 2 characters,
   then parse textures/colors
   else assume it is a map part */
static int	pass1_classify_line(char *line_temp, t_parse_helper *ph)
{
	char	*s;

	s = ft_strtrim(line_temp, " \t\r\n");
	if (!s || s[0] == 0)
	{
		if (s)
			free(s);
		handle_potential_map_end(ph);
	}
	else
	{
		if (ph->found_map_end)
		{
			free(s);
			return (error_return(0, "Empty line in map is not allowed!", -1));
		}
		ph->interpreted_this_line = 0;
		if (pass1_classify_trimmed_line(ph, line_temp, s))
			return (1);
	}
	ph->line_idx++;
	return (0);
}

static void	finalize_gradients_and_fog(t_parse_helper *ph)
{
	ph->game->scene.floor_gradient
		= gradient_increment(ph->game->scene.floor_colour, 0x000000, 100, 20);
	ph->game->scene.fog
		= gradient_increment(ph->game->scene.floor_colour, 0x000000, 100, 25);
	ph->game->scene.ceiling_gradient
		= gradient_increment(ph->game->scene.ceiling_colour, 0x000000, 100, 20);
	if (ph->game->scene.ceiling_colour == DEFAULT_SKY)
		ph->game->scene.ceiling_gradient = DEFAULT_SKY_GRADIENT;
	if (ph->game->scene.floor_colour == DEFAULT_FLOOR)
	{
		ph->game->scene.floor_gradient = DEFAULT_FLOOR_GRADIENT;
		ph->game->scene.fog = DEFAULT_DISTANCE_FADE;
	}
}

/* set map size and map_start_line
 * validate existence of textures and colors */
static int	pass1_finalize(t_parse_helper *ph, int *map_start_line)
{
	int	i;

	handle_potential_map_end(ph);
	*map_start_line = ph->map_start_idx;
	ph->game->scene.map.map_height = ph->map_end_idx - ph->map_start_idx;
	ph->game->scene.map.map_width = ph->map_max_line_length;
	i = 0;
	while (i++ < 4)
		if (!ph->game->scene.textures[i].filename)
			return (error_return(0, "Missing texture!", -1));
	if (!ph->found_ceiling_color || !ph->found_floor_color)
		return (error_return(0, "Missing ceiling or floor color!", -1));
	finalize_gradients_and_fog(ph);
	return (0);
}

/* check if each line is something expected
   record the first line of the map
   record map width and height into the scene */
int	parse_mapfile_pass_1(char *map_fn, t_game *game, int *map_start_line)
{
	int				skip;
	t_parse_helper	ph;

	ft_memset(&ph, 0, sizeof(t_parse_helper));
	ph.game = game;
	ph.map_fd = open(map_fn, O_RDONLY);
	if (ph.map_fd == -1)
		return (error_return_s(1, "Could not open map file ", -1, map_fn));
	skip = 0;
	while (1)
	{
		ph.line_temp = get_next_line(ph.map_fd);
		if (ph.line_temp == NULL)
			break ;
		if (skip == 0 && pass1_classify_line(ph.line_temp, &ph))
			skip = 1;
		free(ph.line_temp);
	}
	close(ph.map_fd);
	if (ph.line_temp == NULL && skip == 0)
		return (pass1_finalize(&ph, map_start_line));
	return (1);
}
