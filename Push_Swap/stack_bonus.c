/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 21:30:00 by gecasas           #+#    #+#             */
/*   Updated: 2026/10/20 21:30:00 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

void	stack_push(t_stack *stack, t_node *node)
{
	if (stack->size == 0)
	{
		node->next = node;
		node->prev = node;
	}
	else
	{
		node->next = stack->top;
		node->prev = stack->top->prev;
		stack->top->prev->next = node;
		stack->top->prev = node;
	}
	stack->top = node;
	stack->size++;
}

void	stack_add_back(t_stack *stack, t_node *node)
{
	stack_push(stack, node);
	stack->top = node->next;
}

t_node	*stack_pop(t_stack *stack)
{
	t_node	*node;

	node = stack->top;
	if (stack->size == 1)
		stack->top = NULL;
	else
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
		stack->top = node->next;
	}
	stack->size--;
	return (node);
}

void	stack_clear(t_stack *stack)
{
	t_node	*node;

	while (stack->size > 0)
	{
		node = stack_pop(stack);
		free(node);
	}
}
