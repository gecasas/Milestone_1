/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_chunks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 18:56:15 by gecasas           #+#    #+#             */
/*   Updated: 2026/10/15 19:42:00 by gecasas          ###   ########.fr       */
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

static void	push_chunk(t_stack *a, t_stack *b, t_move *move, int end)
{
	int	mid;

	mid = b->size + (end - b->size) / 2;
	while (b->size < end)
	{
		if (a->top->index < end)
		{
			pb(a, b);
			move->pb_count++;
			if (b->top->index < mid && b->size > 1)
			{
				rb(b);
				move->rb_count++;
			}
		}
		else
		{
			ra(a);
			move->ra_count++;
		}
	}
}

static void	push_back(t_stack *a, t_stack *b, t_move *move)
{
	int	pos;

	while (b->size > 0)
	{
		pos = get_position(b, b->size - 1);
		if (pos <= b->size / 2)
		{
			while (pos-- > 0)
			{
				rb(b);
				move->rb_count++;
			}
		}
		else
		{
			while (pos++ < b->size)
			{
				rrb(b);
				move->rrb_count++;
			}
		}
		pa(a, b);
		move->pa_count++;
	}
}

void	sort_chunks(t_stack *a, t_stack *b, t_move *move)
{
	int	size;
	int	chunk;
	int	end;

	if (!a || !b || !move || a->size < 2 || is_sorted(a))
		return ;
	size = a->size;
	chunk = int_sqrt(size);
	end = 0;
	while (end < size)
	{
		end += chunk;
		if (end > size)
			end = size;
		push_chunk(a, b, move, end);
	}
	push_back(a, b, move);
}
