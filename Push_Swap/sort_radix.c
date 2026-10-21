/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   sort_radix.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/15 18:56:04 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/21 16:24:34 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	is_sorted(t_stack *stack)
{
	t_node	*node;
	int		i;

	if (!stack || stack->size < 2)
		return (1);
	node = stack->top;
	i = 1;
	while (i < stack->size)
	{
		if (node->value > node->next->value)
			return (0);
		node = node->next;
		i++;
	}
	return (1);
}

static int	get_max_bits(int max)
{
	int	bits;

	bits = 0;
	while ((max >> bits) != 0)
		bits++;
	return (bits);
}

static void	radix_pass(t_context *ctx, int bit)
{
	int	size;
	int	i;

	size = ctx->a->size;
	i = 0;
	while (i < size)
	{
		if ((ctx->a->top->index >> bit) & 1)
		{
			ra(ctx);
			ctx->move->ra_count++;
		}
		else
		{
			pb(ctx);
			ctx->move->pb_count++;
		}
		i++;
	}
	while (ctx->b->size > 0)
	{
		pa(ctx);
		ctx->move->pa_count++;
	}
}

void	sort_radix(t_context *ctx)
{
	int	bits;
	int	bit;

	if (!ctx || !ctx->a || !ctx->b || !ctx->move || ctx->a->size < 2)
		return ;
	bits = get_max_bits(ctx->a->size - 1);
	bit = 0;
	while (bit < bits && !is_sorted(ctx->a))
	{
		radix_pass(ctx, bit);
		bit++;
	}
}
