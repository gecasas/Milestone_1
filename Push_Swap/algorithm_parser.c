/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   algorithm_parser.c                                :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/21 15:11:17 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/22 21:48:03 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

float	compute_disorder(t_stack *a)
{
	t_node	*node;
	int		disordered_pairs;
	int		pair_count;

	if (!a || !a->top || a->size < 2)
		return (0);
	node = a->top;
	disordered_pairs = 0;
	pair_count = 0;
	while (pair_count < a->size - 1)
	{
		if (node->value > node->next->value)
			disordered_pairs++;
		node = node->next;
		pair_count++;
	}
	return ((float) disordered_pairs / pair_count);
}

void	parser(t_context *ctx)
{
	float	disorder;

	if (!ctx->a || !ctx->b)
		return ;
	disorder = compute_disorder(ctx->a);
	ctx->original_disorder = disorder;
	if (disorder < 0.2)
	{
		ctx->algorithm = ALGO_SIMPLE;
		bubble_sort(ctx);
	}
	else if (disorder < 0.5)
	{
		ctx->algorithm = ALGO_MEDIUM;
		sort_chunks(ctx);
	}
	else if (disorder >= 0.5)
	{
		ctx->algorithm = ALGO_COMPLEX;
		sort_radix(ctx);
	}
}

int	parse_algorithm(const char *arg, t_algorithm *algorithm)
{
	if (ft_strncmp(arg, "--simple", 8) == 0)
		*algorithm = ALGO_SIMPLE;
	else if (ft_strncmp(arg, "--medium", 8) == 0)
		*algorithm = ALGO_MEDIUM;
	else if (ft_strncmp(arg, "--complex", 9) == 0)
		*algorithm = ALGO_COMPLEX;
	else if (ft_strncmp(arg, "--adaptive", 10) == 0)
		*algorithm = ALGO_ADAPTIVE;
	else
		return (0);
	return (1);
}

void	run_algorithm(t_algorithm algo, t_context *ctx)
{
	if (algo == ALGO_SIMPLE)
		bubble_sort(ctx);
	else if (algo == ALGO_MEDIUM)
		sort_chunks(ctx);
	else if (algo == ALGO_COMPLEX)
		sort_radix(ctx);
	else if (algo == ALGO_ADAPTIVE)
		parser(ctx);
}
