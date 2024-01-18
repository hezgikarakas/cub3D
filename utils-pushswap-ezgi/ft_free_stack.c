/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_stack.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <hakaraka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 08:33:18 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/02 10:22:29 by hakaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ft_free_stack(t_list **stack)
{
	t_list	*start;
	t_list	*tmp;

	start = *stack;
	while (start)
	{
		tmp = start;
		start = start->next;
		free(tmp);
	}
	free(stack);
}
