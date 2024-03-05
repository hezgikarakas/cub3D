/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pass1_classify.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkatzenb <jkatzenb@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:53:54 by hakaraka          #+#    #+#             */
/*   Updated: 2024/03/05 15:11:01 by jkatzenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/cub3D.h"

int	convert_colour(char *colour_str)
{
	char	**rgb;
	int		r;
	int		g;
	int		b;
	int		error;

	error = 0;

	rgb = ft_split(colour_str, ',');
	if (!rgb[0] || !rgb[1] || !rgb[2] || rgb[3] ||
		string_is_digit(rgb[0]) || string_is_digit(rgb[1]) || string_is_digit(rgb[2]))
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
		return (error_return(0, "Unexpected color (expect 3x 0..255)", -1));
	else
		return (r << 16 | g << 8 | b);
}

int	pass1_parse_color(t_parse_helper *ph, char which, char *rest)
{
	int	converted;

	converted = convert_colour(rest);
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

static int	pass1_parse_texture(t_parse_helper *ph, t_texture *tex, char *rest)
{
	char	*s;
	int		notfound;
	int		fd;

	s = ft_strtrim(rest, " \t\r\n");
	if (tex->filename != NULL)
	{
		free(s);
		return (error_return(0, "Unexpected duplicate texture!", -1));
	}
	tex->filename = s;
	if (!s || s[0] == 0)
		return (error_return(0, "Unexpected empty texture file name!", -1));
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
	if (!ph->found_map_start && s[1] != 0)
	{
		if (s[0] == 'C' || s[0] == 'F')
		{
			if (pass1_parse_color(ph, s[0], s + 1))
			{
				free(s);
				return (1);
			}
		}
		if (pass1_parse_potential_texture(ph, s))
		{
			free(s);
			return (1);
		}
	}
	free(s);
	setmaxlinelen(ph, line_temp);
	return (0);
}
