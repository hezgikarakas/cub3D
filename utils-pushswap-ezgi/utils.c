/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <hakaraka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 11:48:38 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/02 10:22:20 by hakaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

// void	ft_error(char *msg);
// void	ft_free(char **str);
// int		ft_is_sorted(t_list **stack);
// int		ft_get_position(t_list **stack, int idx);
// void	make_the_first(t_list **stack, int position);

void	ft_error(char *msg)
{
	ft_putendl_fd(msg, 2);
	exit(1);
}

void	ft_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	while (i - 1 >= 0)
		if (str[--i])
			free(str[i]);
	if (str)
		free(str);
}

int	ft_is_sorted(t_list **stack)
{
	t_list	*start;

	start = *stack;
	while (start && start->next)
	{
		if (start->value > start->next->value)
			return (0);
		start = start->next;
	}
	return (1);
}

int	ft_get_position(t_list **stack, int idx)
{
	t_list	*start;
	int		position;

	start = *stack;
	position = 0;
	while (start)
	{
		if (start->idx == idx)
			break ;
		position++;
		start = start->next;
	}
	return (position);
}

void	make_the_first(t_list **stack, int position)
{
	t_list	*start;
	int		tmp;

	if (position == 0)
		return ;
	start = *stack;
	tmp = ft_lstsize(start) - position;
	if (position <= (ft_lstsize(start) / 2))
	{
		while (position-- > 0)
			ra(stack);
	}
	else
	{
		while (tmp-- > 0)
			rra(stack);
	}
}
