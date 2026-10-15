/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   algorithms.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/13 14:39:25 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/14 15:05:34 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	bubble_operations(t_stack *a, t_move *move, int *comp, int *rotate)
{
	int	i;
	int	swapped;

	swapped = 0;
	i = -1;
	while (i++ < *comp)
	{
		if (a->top->value > a->top->next->value)
		{
			sa(a);
			move->sa_count++;
			swapped = 1;
		}
		if (i < *comp - 1)
		{
			ra(a);
			move->ra_count++;
			(*rotate)++;
		}
	}
	return (swapped);
}

void	bubble_sort(t_stack *a, t_move *ops)
{
	int	pass;
	int	comparisons;
	int	rotations;
	int	swapped;

	if (!a || a->size < 2)
		return ;
	pass = 0;
	while (pass < a->size - 1)
	{
		rotations = 0;
		comparisons = a->size - 1 - pass;
		swapped = bubble_operations(a, ops, &comparisons, &rotations);
		while (rotations > 0)
		{
			rra(a);
			ops->rra_count++;
			(rotations)--;
		}
		if (!swapped)
			break ;
		pass++;
	}
}
