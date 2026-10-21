/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   argparser.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/16 14:26:23 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/21 15:58:42 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	is_space(char c)
{
	return (c == ' ');
}

static int	is_option(const char *arg)
{
	int	comp;

	comp = ft_strncmp(arg, "--bench", 8);
	if (comp != 0)
		comp = ft_strncmp(arg, "--simple", 9);
	if (comp != 0)
		comp = ft_strncmp(arg, "--medium", 9);
	if (comp != 0)
		comp = ft_strncmp(arg, "--complex", 10);
	return ((comp == 0));
}

static int	parse_number(const char *str, int *num)
{
	long	value;
	int		sign;

	value = 0;
	sign = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (*str < '0' || *str > '9')
		return (0);
	while (*str >= '0' && *str <= '9')
	{
		value = value * 10 + (*str - '0');
		str++;
	}
	if (*str != '\0')
		return (0);
	*num = (int)(value * sign);
	return (1);
}

static int	has_duplicate(t_stack *a, int num)
{
	t_node	*current;

	current = a->top;
	while (current)
	{
		if (current->value == num)
			return (1);
		current = current->next;
		if (current == a->top)
			break ;
	}
	return (0);
}

static int	add_number(t_stack *a, const char *str)
{
	int		num;
	t_node	*node;

	if (!parse_number(str, &num) || has_duplicate(a, num))
		return (0);
	node = new_node(num);
	if (!node)
		return (0);
	stack_add_back(a, node);
	return (1);
}

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

int	argparser(int argc, char **argv, t_stack *a)
{
	int		i;
	char	*str;
	char	*start;

	i = 1;
	while (i < argc)
	{
		if (is_option(argv[i]))
		{
			i++;
			continue ;
		}
		str = argv[i];
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
		i++;
	}
	if (!a->top)
		return (0);
	assign_indices(a);
	return (1);
}
