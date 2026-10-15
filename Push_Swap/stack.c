/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   stack.c                                           :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/10 15:11:48 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/14 15:03:51 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	stack_init(t_stack *stack)
{
	stack->top = NULL;
	stack->size = 0;
}

void	move_init(t_move *move)
{
	move->sa_count = 0;
	move->sb_count = 0;
	move->ss_count = 0;
	move->pa_count = 0;
	move->pb_count = 0;
	move->ra_count = 0;
	move->rb_count = 0;
	move->rr_count = 0;
	move->rra_count = 0;
	move->rrb_count = 0;
	move->rrr_count = 0;
}

t_node	*new_node(int value)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->value = value;
	node->index = -1;
	node->next = node;
	node->prev = node;
	return (node);
}

void	stack_add_back(t_stack *stack, t_node *node)
{
	t_node	*last;

	if (!stack || !node)
		return ;
	if (!stack->top)
	{
		stack->top = node;
		node->next = node;
		node->prev = node;
	}
	else
	{
		last = stack->top->prev;
		last->next = node;
		node->prev = last;
		node->next = stack->top;
		stack->top->prev = node;
	}
	stack->size++;
}

void	stack_clear(t_stack *stack)
{
	t_node	*current;
	t_node	*next;

	if (!stack)
		return ;
	current = stack->top;
	while (stack->size > 0 && current)
	{
		next = current->next;
		free(current);
		current = next;
		stack->size--;
	}
	stack->top = NULL;
	stack->size = 0;
}
