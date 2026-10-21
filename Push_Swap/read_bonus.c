/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 21:30:00 by gecasas           #+#    #+#             */
/*   Updated: 2026/10/20 21:30:00 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

static int	match_op(char *line, int len)
{
	char	*names;
	int		op;
	int		k;

	names = "sa sb ss pa pb ra rb rr rrarrbrrr";
	op = OP_SA;
	while (op <= OP_RRR)
	{
		k = 0;
		while (k < 3 && ((k < len && line[k] == names[op * 3 + k])
				|| (k >= len && names[op * 3 + k] == ' ')))
			k++;
		if (k == 3)
			return (op);
		op++;
	}
	return (-1);
}

static void	add_op(t_checker *c, int op)
{
	char	*new_ops;
	int		i;

	if (c->count == c->capacity)
	{
		c->capacity = c->capacity * 2 + 64;
		new_ops = malloc(c->capacity);
		if (!new_ops)
			error_exit(c);
		i = 0;
		while (i < c->count)
		{
			new_ops[i] = c->ops[i];
			i++;
		}
		free(c->ops);
		c->ops = new_ops;
	}
	c->ops[c->count] = (char)op;
	c->count++;
}

static void	process_char(t_checker *c, char ch)
{
	int	op;

	if (ch == '\n')
	{
		op = match_op(c->line, c->len);
		if (op < 0)
			error_exit(c);
		add_op(c, op);
		c->len = 0;
	}
	else if (c->len < 3 && ch != '\0' && ft_strchr("abprs", ch))
	{
		c->line[c->len] = ch;
		c->len++;
	}
	else
		error_exit(c);
}

void	read_instructions(t_checker *c)
{
	char	buf[READ_SIZE];
	ssize_t	bytes;
	ssize_t	i;

	bytes = read(0, buf, READ_SIZE);
	while (bytes > 0)
	{
		i = 0;
		while (i < bytes)
		{
			process_char(c, buf[i]);
			i++;
		}
		bytes = read(0, buf, READ_SIZE);
	}
	if (bytes < 0 || c->len != 0)
		error_exit(c);
}
