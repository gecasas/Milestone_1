/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   rotate_operations.c                               :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/12 15:40:33 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/13 13:58:00 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ra(t_stack *a)
{
	if (!a || a->size < 2)
		return ;
	a->top = a->top->next;
}

void	rb(t_stack *b)
{
	if (!b || b->size < 2)
		return ;
	b->top = b->top->next;
}

void	rr(t_stack *a, t_stack *b)
{
	ra(a);
	rb(b);
}
