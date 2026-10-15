/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   reverse_rotate_operations.c                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/13 13:52:21 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/13 13:55:18 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	rra(t_stack *a)
{
	if (!a || a->size < 2)
		return ;
	a->top = a->top->prev;
}

void	rrb(t_stack *b)
{
	if (!b || b->size < 2)
		return ;
	b->top = b->top->prev;
}

void	rrr(t_stack *a, t_stack *b)
{
	rra(a);
	rrb(b);
}
