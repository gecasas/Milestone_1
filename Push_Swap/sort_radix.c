/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_radix.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 18:56:04 by gecasas           #+#    #+#             */
/*   Updated: 2026/10/15 19:41:35 by gecasas          ###   ########.fr       */
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

static void	radix_pass(t_stack *a, t_stack *b, t_move *move, int bit)
{
	int	size;
	int	i;

	size = a->size;
	i = 0;
	while (i < size)
	{
		if ((a->top->index >> bit) & 1)
		{
			ra(a);
			move->ra_count++;
		}
		else
		{
			pb(a, b);
			move->pb_count++;
		}
		i++;
	}
	while (b->size > 0)
	{
		pa(a, b);
		move->pa_count++;
	}
}

void	sort_radix(t_stack *a, t_stack *b, t_move *move)
{
	int	bits;
	int	bit;

	if (!a || !b || !move || a->size < 2)
		return ;
	bits = get_max_bits(a->size - 1);
	bit = 0;
	while (bit < bits && !is_sorted(a))
	{
		radix_pass(a, b, move, bit);
		bit++;
	}
}
