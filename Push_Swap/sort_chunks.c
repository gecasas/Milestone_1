/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   sort_chunks.c                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/15 18:56:15 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/21 15:48:02 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	int_sqrt(int n)
{
	int	root;

	root = 1;
	while ((root + 1) * (root + 1) <= n)
		root++;
	return (root);
}

static int	get_position(t_stack *stack, int index)
{
	t_node	*node;
	int		pos;

	node = stack->top;
	pos = 0;
	while (pos < stack->size)
	{
		if (node->index == index)
			return (pos);
		node = node->next;
		pos++;
	}
	return (-1);
}

static void	push_chunk(t_context *ctx, int end)
{
	int	mid;

	mid = ctx->b->size + (end - ctx->b->size) / 2;
	while (ctx->b->size < end)
	{
		if (ctx->a->top->index < end)
		{
			pb(ctx);
			ctx->move->pb_count++;
			if (ctx->b->top->index < mid && ctx->b->size > 1)
			{
				rb(ctx);
				ctx->move->rb_count++;
			}
		}
		else
		{
			ra(ctx);
			ctx->move->ra_count++;
		}
	}
}

static void	push_back(t_context *ctx)
{
	int	pos;

	while (ctx->b->size > 0)
	{
		pos = get_position(ctx->b, ctx->b->size - 1);
		if (pos <= ctx->b->size / 2)
		{
			while (pos-- > 0)
			{
				rb(ctx);
				ctx->move->rb_count++;
			}
		}
		else
		{
			while (pos++ < ctx->b->size)
			{
				rrb(ctx);
				ctx->move->rrb_count++;
			}
		}
		pa(ctx);
		ctx->move->pa_count++;
	}
}

void	sort_chunks(t_context *ctx)
{
	int	size;
	int	chunk;
	int	end;

	if (!ctx || !ctx->a || !ctx->b || !ctx->move || ctx->a->size < 2 || is_sorted(ctx->a))
		return ;
	size = ctx->a->size;
	chunk = int_sqrt(size);
	end = 0;
	while (end < size)
	{
		end += chunk;
		if (end > size)
			end = size;
		push_chunk(ctx, end);
	}
	push_back(ctx);
}
