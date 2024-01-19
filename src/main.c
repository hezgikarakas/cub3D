/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karakasschueller <karakasschueller@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 12:47:02 by karakasschu       #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int main(int an, char** ac)
{
	char*		map_fn;
	t_game		game;

	ft_memset(&game, 0, sizeof(t_game));
	if (interpret_arguments(an, ac, &game, &map_fn))
		return (print_return_error(&game));

	return (0);
}