/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   push_swap.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/06 11:50:22 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/14 18:32:01 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"
#include <stdio.h>

static void	print_stack(t_stack *stack)
{
	t_node	*current;
	int		i;

	if (!stack || !stack->top)
	{
		printf("(empty)\n");
		return ;
	}
	current = stack->top;
	printf("[ ");
	i = 0;
	while (i < stack->size)
	{
		printf("%d", current->value);
		if (i < stack->size - 1)
			printf(", ");
		current = current->next;
		i++;
	}
	printf(" ]\n");
}

int	compute_disorder(t_stack *a, t_stack *b)
{
	t_node	*node_a;
	t_node	*node_b;
	int		same_values;
	int		total_values;

	if (!a || !b)
		return (0);
	node_a = a->top;
	node_b = b->top;
	same_values = 0;
	total_values = 0;
	while (node_a && node_b && total_values < a->size && total_values < b->size)
	{
		if (node_a->value == node_b->value)
			same_values++;
		node_a = node_a->next;
		node_b = node_b->next;
		total_values++;
	}
	if (total_values == 0)
		return (0);
	return ((same_values * 100) / total_values);
}

/*void	parser(t_stack *a, t_stack *b)
{
int	order;
if (!a || !b)
return ;
order = compute_disorder(a, b);
if (order < 20)
bubble_sort(a);
else if (order < 50)
return ;
else
return ;
}*/

int	main(void)
{
	t_stack	a;
	t_node	*node;
	t_move	ops;

	stack_init(&a);
	move_init(&ops);
	node = new_node(5);
	stack_add_back(&a, node);
	node = new_node(2);
	stack_add_back(&a, node);
	node = new_node(4);
	stack_add_back(&a, node);
	node = new_node(1);
	stack_add_back(&a, node);
	node = new_node(3);
	stack_add_back(&a, node);
	printf("Before:\n");
	print_stack(&a);
	bubble_sort(&a, &ops);
	printf("After:\n");
	print_stack(&a);
	stack_clear(&a);
	printf("Operations: %d\n", ops.sa_count + ops.sb_count + ops.ss_count + ops.pa_count + ops.pb_count + ops.ra_count + ops.rb_count + ops.rr_count + ops.rra_count + ops.rrb_count + ops.rrr_count);
	return (0);
}
