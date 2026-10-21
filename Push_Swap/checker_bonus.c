/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 21:30:00 by gecasas           #+#    #+#             */
/*   Updated: 2026/10/20 21:30:00 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

void	free_checker(t_checker *c)
{
	stack_clear(&c->a);
	stack_clear(&c->b);
	free(c->ops);
	c->ops = NULL;
}

void	error_exit(t_checker *c)
{
	free_checker(c);
	write(2, "Error\n", 6);
	exit(1);
}

static void	init_checker(t_checker *c)
{
	c->a.top = NULL;
	c->a.size = 0;
	c->b.top = NULL;
	c->b.size = 0;
	c->ops = NULL;
	c->count = 0;
	c->capacity = 0;
	c->len = 0;
}

static int	is_sorted(t_stack *stack)
{
	t_node	*node;
	int		i;

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

int	main(int argc, char **argv)
{
	t_checker	c;
	int			i;

	if (argc < 2)
		return (0);
	init_checker(&c);
	parse_args(&c, argc, argv);
	read_instructions(&c);
	i = 0;
	while (i < c.count)
	{
		apply_op(&c, c.ops[i]);
		i++;
	}
	if (is_sorted(&c.a) && c.b.size == 0)
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	free_checker(&c);
	return (0);
}
