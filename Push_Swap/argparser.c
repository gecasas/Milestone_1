/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   argparser.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/16 14:26:23 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/22 14:18:03 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	assign_indices(t_stack *a)
{
	t_node	*node;
	t_node	*other;
	int		i;
	int		j;
	int		index;

	node = a->top;
	i = 0;
	while (i < a->size)
	{
		index = 0;
		other = a->top;
		j = 0;
		while (j < a->size)
		{
			if (other->value < node->value)
				index++;
			other = other->next;
			j++;
		}
		node->index = index;
		node = node->next;
		i++;
	}
}

int	parser_while(char *str, char *start, t_stack *a)
{
	while (*str)
	{
		while (is_space(*str))
			str++;
		if (*str == '\0')
			break ;
		start = str;
		while (*str && !is_space(*str))
			str++;
		if (*str)
			*str++ = '\0';
		if (!add_number(a, start))
			return (0);
	}
	return (1);
}

int	argparser(int argc, char **argv, t_stack *a)
{
	int		i;
	char	*str;
	char	*start;

	i = 1;
	start = NULL;
	while (i < argc)
	{
		if (is_option(argv[i]))
		{
			i++;
			continue ;
		}
		str = argv[i];
		if (!parser_while(str, start, a))
			return (0);
		i++;
	}
	if (!a->top)
		return (0);
	assign_indices(a);
	return (1);
}
