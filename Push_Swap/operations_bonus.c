/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 21:30:00 by gecasas           #+#    #+#             */
/*   Updated: 2026/10/20 21:30:00 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

static void	swap(t_stack *s)
{
	int	tmp;

	if (s->size < 2)
		return ;
	tmp = s->top->value;
	s->top->value = s->top->next->value;
	s->top->next->value = tmp;
}

static void	push(t_stack *dst, t_stack *src)
{
	if (src->size == 0)
		return ;
	stack_push(dst, stack_pop(src));
}

static void	rotate(t_stack *s)
{
	if (s->size > 1)
		s->top = s->top->next;
}

static void	reverse_rotate(t_stack *s)
{
	if (s->size > 1)
		s->top = s->top->prev;
}

void	apply_op(t_checker *c, int op)
{
	if (op == OP_SA || op == OP_SS)
		swap(&c->a);
	if (op == OP_SB || op == OP_SS)
		swap(&c->b);
	if (op == OP_PA)
		push(&c->a, &c->b);
	if (op == OP_PB)
		push(&c->b, &c->a);
	if (op == OP_RA || op == OP_RR)
		rotate(&c->a);
	if (op == OP_RB || op == OP_RR)
		rotate(&c->b);
	if (op == OP_RRA || op == OP_RRR)
		reverse_rotate(&c->a);
	if (op == OP_RRB || op == OP_RRR)
		reverse_rotate(&c->b);
}
