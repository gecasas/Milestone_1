/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   push_swap.h                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/06 11:47:38 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/22 22:54:37 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft/libft.h"
# include "libft/printf/ft_printf.h"
# include <stdio.h>

typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;

typedef struct s_stack
{
	t_node	*top;
	int		size;
}	t_stack;

typedef struct s_move
{
	int	sa_count;
	int	sb_count;
	int	ss_count;
	int	pa_count;
	int	pb_count;
	int	ra_count;
	int	rb_count;
	int	rr_count;
	int	rra_count;
	int	rrb_count;
	int	rrr_count;
}	t_move;

typedef enum e_algorithm
{
	ALGO_SIMPLE,
	ALGO_MEDIUM,
	ALGO_COMPLEX,
	ALGO_ADAPTIVE
}	t_algorithm;

typedef struct s_context
{
	t_stack		*a;
	t_stack		*b;
	t_move		*move;
	t_list		*instructions;
	t_list		*instructions_tail;
	t_algorithm	algorithm;
	int			has_algorithm;
	int			total_ops;
	int			bench_mode;
	float		original_disorder;
}	t_context;

//Flags and argument parsing
int		is_sorted(t_stack *stack);
void	append_instruction(t_context *ctx, char *op);
void	print_instructions(t_context *ctx);
void	clear_instr_list(t_context *ctx);

//Parser Utils
int		is_space(char c);
int		is_option(const char *arg);
int		parse_number(const char *str, int *num);
int		has_duplicate(t_stack *a, int num);
int		add_number(t_stack *a, const char *str);

//Algorithm functions
float	compute_disorder(t_stack *a);
void	parser(t_context *ctx);
int		argparser(int argc, char **argv, t_stack *a);
void	run_algorithm(t_algorithm algo, t_context *ctx);
int		parse_algorithm(const char *arg, t_algorithm *algorithm);

//Stack functions
t_node	*new_node(int value);
void	stack_init(t_stack *stack);
void	move_init(t_move *move);
void	stack_add_back(t_stack *stack, t_node *node);
void	stack_clear(t_stack *stack);
void	stack_swap(t_stack *stack);
void	stack_rotate(t_stack *stack);
void	stack_reverse_rotate(t_stack *stack);

//Stack operations
void	sa(t_context *ctx);
void	sb(t_context *ctx);
void	ss(t_context *ctx);
void	pa(t_context *ctx);
void	pb(t_context *ctx);
void	ra(t_context *ctx);
void	rb(t_context *ctx);
void	rr(t_context *ctx);
void	rra(t_context *ctx);
void	rrb(t_context *ctx);
void	rrr(t_context *ctx);

//Push functions
t_node	*stack_pop(t_stack *stack);
void	stack_push(t_stack *stack, t_node *node);

//Algorithms
void	bubble_sort(t_context *ctx);
void	sort_radix(t_context *ctx);
void	sort_chunks(t_context *ctx);
int		is_sorted(t_stack *stack);

#endif
