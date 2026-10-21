/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 21:30:00 by gecasas           #+#    #+#             */
/*   Updated: 2026/10/20 21:30:00 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKER_BONUS_H
# define CHECKER_BONUS_H

# include <stdlib.h>
# include <unistd.h>
# include "libft.h"

# define READ_SIZE 4096

typedef enum e_op
{
	OP_SA,
	OP_SB,
	OP_SS,
	OP_PA,
	OP_PB,
	OP_RA,
	OP_RB,
	OP_RR,
	OP_RRA,
	OP_RRB,
	OP_RRR
}	t_op;

typedef struct s_node
{
	int				value;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;

typedef struct s_stack
{
	t_node	*top;
	int		size;
}	t_stack;

typedef struct s_checker
{
	t_stack	a;
	t_stack	b;
	char	*ops;
	int		count;
	int		capacity;
	char	line[3];
	int		len;
}	t_checker;

void	error_exit(t_checker *c);
void	free_checker(t_checker *c);
void	parse_args(t_checker *c, int argc, char **argv);
void	read_instructions(t_checker *c);
void	apply_op(t_checker *c, int op);
void	stack_add_back(t_stack *stack, t_node *node);
void	stack_push(t_stack *stack, t_node *node);
t_node	*stack_pop(t_stack *stack);
void	stack_clear(t_stack *stack);

#endif
