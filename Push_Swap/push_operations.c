/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   push_operations.c                                 :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/10 16:52:38 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/10 16:53:09 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_node	*stack_pop(t_stack *stack)
{
	t_node	*node;
	t_node	*last;
	t_node	*next;

	if (!stack || stack->size == 0)
		return (NULL);
	node = stack->top;
	if (stack->size == 1)
		stack->top = NULL;
	else
	{
		last = node->prev;
		next = node->next;
		last->next = next;
		next->prev = last;
		stack->top = next;
	}
	stack->size--;
	node->next = node;
	node->prev = node;
	return (node);
}

void	stack_push(t_stack *stack, t_node *node)
{
	t_node	*last;

	if (!stack || !node)
		return ;
	if (stack->size == 0)
	{
		stack->top = node;
		node->next = node;
		node->prev = node;
	}
	else
	{
		last = stack->top->prev;
		node->next = stack->top;
		node->prev = last;
		last->next = node;
		stack->top->prev = node;
		stack->top = node;
	}
	stack->size++;
}

void	pa(t_stack *a, t_stack *b)
{
	t_node	*node;

	if (!a || !b || b->size == 0)
		return ;
	node = stack_pop(b);
	stack_push(a, node);
}

void	pb(t_stack *a, t_stack *b)
{
	t_node	*node;

	if (!a || !b || a->size == 0)
		return ;
	node = stack_pop(a);
	stack_push(b, node);
}
