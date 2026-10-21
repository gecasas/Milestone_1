/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   algorithms.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/13 14:39:25 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/21 15:52:43 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	bubble_operations(t_context *ctx, int *comp, int *rotate)
{
	int	i;
	int	swapped;

	swapped = 0;
	i = -1;
	while (i++ < *comp)
	{
		if (ctx->a->top->value > ctx->a->top->next->value)
		{
			sa(ctx);
			ctx->move->sa_count++;
			swapped = 1;
		}
		if (i < *comp - 1)
		{
			ra(ctx);
			ctx->move->ra_count++;
			(*rotate)++;
		}
	}
	return (swapped);
}

void	bubble_sort(t_context *ctx)
{
	int	pass;
	int	comparisons;
	int	rotations;
	int	swapped;

	if (!ctx || !ctx->a || ctx->a->size < 2)
		return ;
	pass = 0;
	while (pass < ctx->a->size - 1)
	{
		rotations = 0;
		comparisons = ctx->a->size - 1 - pass;
		swapped = bubble_operations(ctx, &comparisons, &rotations);
		while (rotations > 0)
		{
			rra(ctx);
			ctx->move->rra_count++;
			(rotations)--;
		}
		if (!swapped)
			break ;
		pass++;
	}
}
