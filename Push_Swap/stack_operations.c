/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   stack_operations.c                                :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/10 15:35:47 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/21 15:47:17 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	stack_swap(t_stack *stack)
{
	t_node	*first;
	t_node	*second;
	t_node	*last;
	t_node	*third;

	if (!stack || stack->size < 2)
		return ;
	first = stack->top;
	second = first->next;
	if (stack->size == 2)
	{
		stack->top = second;
		return ;
	}
	last = first->prev;
	third = second->next;
	last->next = second;
	second->prev = last;
	second->next = first;
	first->prev = second;
	first->next = third;
	third->prev = first;
	stack->top = second;
}

void	sa(t_context *ctx)
{
	if (!ctx || !ctx->a || ctx->a->size < 2)
		return ;
	append_instruction(ctx, "sa");
	stack_swap(ctx->a);
}

void	sb(t_context *ctx)
{
	if (!ctx || !ctx->b || ctx->b->size < 2)
		return ;
	append_instruction(ctx, "sb");
	stack_swap(ctx->b);
}

void	ss(t_context *ctx)
{
	if (!ctx || !ctx->a || ctx->a->size < 2 || !ctx->b || ctx->b->size < 2)
		return ;
	append_instruction(ctx, "ss");
	stack_swap(ctx->a);
	stack_swap(ctx->b);
}
