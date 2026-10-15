/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 11:47:38 by abarba-m          #+#    #+#             */
/*   Updated: 2026/10/15 19:44:31 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "Libft/libft.h"

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

typedef struct s_context
{
	t_stack	*a;
	t_stack	*b;
	t_move	*move;
	t_list	*instructions;
	t_list	*instructions_tail;
}	t_context;

//Stack functions
t_node	*new_node(int value);
void	stack_init(t_stack *stack);
void	move_init(t_move *move);
void	stack_add_back(t_stack *stack, t_node *node);
void	stack_clear(t_stack *stack);

//Stack operations
void	sa(t_stack *a);
void	sb(t_stack *b);
void	ss(t_stack *a, t_stack *b);
void	pa(t_stack *a, t_stack *b);
void	pb(t_stack *a, t_stack *b);
void	ra(t_stack *a);
void	rb(t_stack *b);
void	rr(t_stack *a, t_stack *b);
void	rra(t_stack *a);
void	rrb(t_stack *b);
void	rrr(t_stack *a, t_stack *b);

//Push functions
t_node	*stack_pop(t_stack *stack);
void	stack_push(t_stack *stack, t_node *node);

//Swap functions
int		compute_disorder(t_stack *a, t_stack *b);
void	parser(t_stack *a, t_stack *b);

//Algorithms
void	bubble_sort(t_stack *a, t_move *ops);
void	sort_radix(t_stack *a, t_stack *b, t_move *move);
void	sort_chunks(t_stack *a, t_stack *b, t_move *move);
int		is_sorted(t_stack *stack);

#endif
