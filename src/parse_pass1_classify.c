/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pass1_classify.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by karakasschu       #+#    #+#             */
/*   Updated: 2024/02/13 17:40:16 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

int	pass1_parse_col_parts(
	t_parse_helper *ph, int *col, char *rest, char **parts)
{
	int		i;
	long	colortemp;

	i = 0;
	while (parts[i] && i < 3)
	{
		colortemp = ft_atoi(parts[i]);
		if (colortemp <= 0 || colortemp >= 256)
		{
			free_strings(parts);
			return (error_return_s(0, "Unexpected color: ", -1, parts[i]));
		}
		*col = (int)colortemp;
		++col;
		i++;
	}
	if (i < 3 || (i == 3 && parts[i]))
	{
		free_strings(parts);
		return (error_return_s(0, "Expect 3 color components "
				"found the following color spec ", -1, rest));
	}
	free_strings(parts);
	ph->interpreted_this_line = 1;
	return (0);
}

int	convert_colour(char *colour_str)
{
	char	**rgb;
	int		r;
	int		g;
	int		b;

	rgb = ft_split(colour_str, ',');
	r = ft_atoi(rgb[0]);
	g = ft_atoi(rgb[1]);
	b = ft_atoi(rgb[2]);
	free (rgb[0]);
	free (rgb[1]);
	free (rgb[2]);
	free (rgb);
	return (r << 16 | g << 8 | b);
}
/*
void	get_colours(t_game *game)
{
	game->scene.fog = DEFAULT_DISTANCE_FADE;
	if (!game->scene.ceiling_str)
	{
		printf("couldn't find ceiling colour, using default\n");
		game->scene.ceiling_colour = DEFAULT_SKY;
		game->scene.ceiling_gradient = DEFAULT_SKY_GRADIENT;
	}
	else
	{
		game->scene.ceiling_colour = convert_colour(game->scene.ceiling_str);
		game->scene.ceiling_gradient = 0x95c1c1;
		game->scene.fog = 0x95c1c1;
	}
	if (!game->scene.floor_str)
	{
		printf("couldn't find floor colour, using default\n");
		game->scene.floor_colour = DEFAULT_FLOOR;
		game->scene.floor_gradient = DEFAULT_FLOOR_GRADIENT;
	}
	else
	{
		game->scene.floor_colour = convert_colour(game->scene.floor_str);
		game->scene.floor_gradient = 0x95c1c1;
		game->scene.fog = 0x95c1c1;
	}
}
*/

int	pass1_parse_color(t_parse_helper *ph, char which, char *rest)
{
	char	**parts;
	int		*found_color;
	int		*color;

	if (which == 'F')
	{
		// ph->game->scene.floor_colour = convert_colour(rest);
		color = &ph->game->scene.floor_colour;
		found_color = &ph->found_floor_color;
	}
	else if (which == 'C')
	{
		// ph->game->scene.ceiling_colour = convert_colour(rest);
		color = &ph->game->scene.ceiling_colour;
		found_color = &ph->found_ceiling_color;
	}
	else
		return (error_return(0, "Unexpected color line", -1));
	*found_color = 1;
	parts = ft_split(rest, ',');
	if (!parts)
		return (error_return(1, "malloc error", -1));
	return (pass1_parse_col_parts(ph, color, rest, parts));
}

static int	pass1_parse_texture(t_parse_helper *ph, t_texture *tex, char *rest)
{
	char	*s;

	s = ft_strtrim(rest, " \t\r\n");
	if (!s)
		return (error_return(0, "Unexpected empty texture file name!", -1));
	if (ft_strncmp(s + ft_strlen(s) - 4, ".xpm", 4))
		return (error_return_s(0,
				"texture file must have .xpm ending, found ", -1, s));
	tex->filename = s;
	ph->interpreted_this_line = 1;
	return (0);
}

/* detect and parse textures
   indexes see scene.c */
static int	pass1_parse_potential_texture(t_parse_helper *ph, char *s)
{
	if (s[0] == 'N' && s[1] == 'O')
		if (pass1_parse_texture(ph, &ph->game->scene.textures[0], s + 2))
			return (1);
	if (s[0] == 'E' && s[1] == 'A')
		if (pass1_parse_texture(ph, &ph->game->scene.textures[1], s + 2))
			return (1);
	if (s[0] == 'S' && s[1] == 'O')
		if (pass1_parse_texture(ph, &ph->game->scene.textures[2], s + 2))
			return (1);
	if (s[0] == 'W' && s[1] == 'E')
		if (pass1_parse_texture(ph, &ph->game->scene.textures[3], s + 2))
			return (1);
	return (0);
}

/* handle colors and textures, find map start line and map width
 * everything we do not interpret here should be a map
 * if this is not the case, we will detect in pass 2 */
int	pass1_classify_trimmed_line(t_parse_helper *ph, char *line_temp, char *s)
{
	if (!ph->found_map_start && s[1] != 0 && s[2] != 0)
	{
		if (s[0] == 'C' || s[0] == 'F')
			if (pass1_parse_color(ph, s[0], s + 1))
				return (1);
		if (pass1_parse_potential_texture(ph, s))
			return (1);
	}
	free(s);
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
	return (0);
}
