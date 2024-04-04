/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pass1_classify.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by hakaraka          #+#    #+#             */
/*   Updated: 2024/04/04 16:41:59 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	convert_colour(char *colour_str)
{
	char	**rgb;
	int		r;
	int		g;
	int		b;
	int		error;

	error = 0;
	rgb = ft_split(colour_str, ',');
	if (!rgb[0] || !rgb[1] || !rgb[2] || rgb[3]
		|| str_is_digit(rgb[0]) || str_is_digit(rgb[1]) || str_is_digit(rgb[2]))
		error = 1;
	else
	{
		r = ft_atoi(rgb[0]);
		g = ft_atoi(rgb[1]);
		b = ft_atoi(rgb[2]);
		if (r < 0 || g < 0 || b < 0 || r > 255 || g > 255 || b > 255)
			error = 1;
	}
	free_strings(rgb);
	if (error)
		return (error_return(0, "Unexpected color\
			\n(expect 3x 0..255 separated by commas, no whitespaces)", -1));
	else
		return (r << 16 | g << 8 | b);
}

int	pass1_parse_color(t_parse_helper *ph, char which, char *rest)
{
	int	converted;

	if (!is_ws(rest[0]))
		return (error_return(0, "Malformed color line!\
			\nMust be one space between letter and color values", -1));
	converted = convert_colour(rest + nmbr_ws(rest));
	if (converted < 0)
		return (converted);
	if (which == 'F' && !ph->found_floor_color)
	{
		ph->found_floor_color = 1;
		ph->game->scene.floor_colour = converted;
	}
	else if (which == 'C' && !ph->found_ceiling_color)
	{
		ph->found_ceiling_color = 1;
		ph->game->scene.ceiling_colour = converted;
	}
	else
		return (error_return(0, "Unexpected or duplicate color line", -1));
	ph->interpreted_this_line = 1;
	return (0);
}

static int	interpret_texture(t_parse_helper *ph, t_texture *tex, char *rest)
{
	char	*s;
	int		notfound;
	int		fd;

	if (tex->filename != NULL)
		return (error_return(0, "Unexpected duplicate texture!", -1));
	s = ft_strtrim(rest, " \t\r\n");
	tex->filename = s;
	if (!s || s[0] == 0)
		return (error_return(0, "OOM or empty texture file name!", -1));
	if (ft_strlen(s) < 4 || ft_strncmp(s + ft_strlen(s) - 4, ".xpm", 4))
		return (error_return_s(0, "Bad texture format (need .xpm) ", -1, s));
	fd = open(s, O_RDONLY);
	notfound = 0;
	if (!s || fd == -1)
		notfound = 1;
	else
		close(fd);
	if (notfound == 1)
		return (error_return_s(0, "Could not open texture file ", -1, s));
	ph->interpreted_this_line = 1;
	return (0);
}

/* detect and parse textures
   indexes see scene.c */
static int	pass1_parse_texture(t_parse_helper *ph, char *s)
{
	if (s[0] == 'N' && (
			s[1] != 'O' || !is_ws(s[2])
			|| interpret_texture(ph, &ph->game->scene.textures[0], s + 3)))
		return (error_return(0, "Malformed NO texture line!", -1));
	if (s[0] == 'E' && (
			s[1] != 'A' || !is_ws(s[2])
			|| interpret_texture(ph, &ph->game->scene.textures[1], s + 3)))
		return (error_return(0, "Malformed EA texture line!", -1));
	if (s[0] == 'S' && (
			s[1] != 'O' || !is_ws(s[2])
			|| interpret_texture(ph, &ph->game->scene.textures[2], s + 3)))
		return (error_return(0, "Malformed SO texture line!", -1));
	if (s[0] == 'W' && (
			s[1] != 'E' || !is_ws(s[2])
			|| interpret_texture(ph, &ph->game->scene.textures[3], s + 3)))
		return (error_return(0, "Malformed WE texture line!", -1));
	return (0);
}

/* handle colors and textures, find map start line and map width
 * everything we do not interpret here should be a map
 * if this is not the case, we will detect in pass 2 */
int	pass1_classify_trimmed_line(t_parse_helper *ph, char *line_temp, char *s)
{
	if (!ph->found_map_start)
	{
		if (s[0] == 'C' || s[0] == 'F')
		{
			if (pass1_parse_color(ph, s[0], s + 1))
				return (free(s), 1);
		}
		else if (s[0] == 'N' || s[0] == 'S' || s[0] == 'W' || s[0] == 'E')
		{
			if (pass1_parse_texture(ph, s))
				return (free(s), 1);
		}
		else if (s[0] != '1')
		{
			free(s);
			return (error_return(0, "Unexpected line!", -1));
		}
	}
	free(s);
	setmaxlinelen(ph, line_temp);
	return (0);
}
