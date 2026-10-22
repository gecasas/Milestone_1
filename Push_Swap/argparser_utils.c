/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   argparser_utils.c                                 :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/22 13:44:37 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/22 14:04:48 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	is_space(char c)
{
	return (c == ' ');
}

int	is_option(const char *arg)
{
	int	comp;

	comp = ft_strncmp(arg, "--bench", 8);
	if (comp != 0)
		comp = ft_strncmp(arg, "--simple", 9);
	if (comp != 0)
		comp = ft_strncmp(arg, "--medium", 9);
	if (comp != 0)
		comp = ft_strncmp(arg, "--complex", 10);
	if (comp != 0)
		comp = ft_strncmp(arg, "--adaptive", 11);
	return ((comp == 0));
}

int	parse_number(const char *str, int *num)
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

int	has_duplicate(t_stack *a, int num)
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

int	add_number(t_stack *a, const char *str)
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
