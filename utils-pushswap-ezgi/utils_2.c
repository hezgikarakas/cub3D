/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <hakaraka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 08:33:18 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/02 10:22:29 by hakaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

// void	ft_printlst(t_list *lst);
// int		ft_isnum(char *nbr);
// int		ft_has_duplicate_nbr(int nbr, char **av, int i);

void	ft_printlst(t_list *lst)
{
	t_list	*tmp;

	tmp = lst;
	while (tmp != NULL)
	{
		ft_putnbr_fd(tmp->value, 1);
		ft_putendl_fd("", 1);
		tmp = tmp->next;
	}
}

int	ft_isnum(char *nbr)
{
	int	i;

	i = 0;
	if (nbr[0] == '-' || nbr[0] == '+')
	{
		if (nbr[1] == 0)
			return (0);
		i++;
	}
	while (nbr[i])
	{
		if (!ft_isdigit(nbr[i]))
			return (0);
		i++;
	}
	return (1);
}

/* check if av contains nbr using atoi starting from index i */
int	ft_has_duplicate_nbr(int nbr, char **av, int i)
{
	while (av[i])
	{
		if (ft_atoi(av[i]) == nbr)
			return (1);
		i++;
	}
	return (0);
}

int	is_true_integer(long tmp, char *arg)
{
	char	*converted;
	int		ret;

	converted = ft_itoa(tmp);
	if (converted == NULL)
		ft_error("Error: malloc failed\n");
	ret = !ft_strncmp(converted, arg, ft_strlen(arg));
	free(converted);
	return (ret);
}

void	ft_sorting(t_list **stack_a, t_list **stack_b)
{
	if (ft_lstsize(*stack_a) < 6)
		sort_manually(stack_a, stack_b);
	else
		radix_sort(stack_a, stack_b);
}
