/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 21:30:00 by gecasas           #+#    #+#             */
/*   Updated: 2026/10/20 21:30:00 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

static int	is_space(char ch)
{
	return (ch == ' ' || (ch >= '\t' && ch <= '\r'));
}

static int	parse_number(t_checker *c, char *s, int *i)
{
	long	n;
	int		sign;

	sign = 1;
	if (s[*i] == '+' || s[*i] == '-')
	{
		if (s[*i] == '-')
			sign = -1;
		(*i)++;
	}
	if (!ft_isdigit(s[*i]))
		error_exit(c);
	n = 0;
	while (ft_isdigit(s[*i]))
	{
		n = n * 10 + (s[*i] - '0');
		if (n > 2147483648L)
			error_exit(c);
		(*i)++;
	}
	if ((s[*i] != '\0' && !is_space(s[*i])) || n * sign > 2147483647L)
		error_exit(c);
	return ((int)(n * sign));
}

static void	add_number(t_checker *c, int value)
{
	t_node	*node;
	int		i;

	node = c->a.top;
	i = 0;
	while (i < c->a.size)
	{
		if (node->value == value)
			error_exit(c);
		node = node->next;
		i++;
	}
	node = malloc(sizeof(t_node));
	if (!node)
		error_exit(c);
	node->value = value;
	stack_add_back(&c->a, node);
}

static void	parse_arg(t_checker *c, char *s)
{
	int	i;
	int	found;

	i = 0;
	found = 0;
	while (s[i] != '\0')
	{
		while (is_space(s[i]))
			i++;
		if (s[i] == '\0')
			break ;
		add_number(c, parse_number(c, s, &i));
		found = 1;
	}
	if (!found)
		error_exit(c);
}

void	parse_args(t_checker *c, int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		parse_arg(c, argv[i]);
		i++;
	}
}
