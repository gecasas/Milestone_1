/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   push_swap.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/06 11:50:22 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/21 17:15:48 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	bench_print(t_context *ctx)
{
	if (!ctx)
		return ;
	ft_printf("[Bench] disorder: %f\n", compute_disorder(ctx->a));
	ft_printf("[Bench] strategy: %d\n", ctx->algorithm);
	ft_printf("[Bench] sb: %d", ctx->move->sb_count);
	ft_printf(" ss: %d ", ctx->move->ss_count);
	ft_printf(" pa: %d ", ctx->move->pa_count);
	ft_printf(" pb: %d\n", ctx->move->pb_count);
	ft_printf("[Bench] ra: %d", ctx->move->ra_count);
	ft_printf(" rb: %d ", ctx->move->rb_count);
	ft_printf(" rr: %d ", ctx->move->rr_count);
	ft_printf(" rra: %d ", ctx->move->rra_count);
	ft_printf(" rrb: %d ", ctx->move->rrb_count);
	ft_printf(" rrr: %d\n", ctx->move->rrr_count);
	clear_instruction_list(ctx);
}

void	init_context(t_context *ctx)
{
	if (!ctx)
		return ;
	stack_init(ctx->a);
	stack_init(ctx->b);
	move_init(ctx->move);
	ctx->instructions = NULL;
	ctx->instructions_tail = NULL;
	ctx->bench_mode = 0;
	ctx->algorithm = ALGO_ADAPTIVE;
	ctx->has_algorithm = 0;
}

int	main(int argc, char **argv)
{
	t_context	ctx;
	t_algorithm	algorithm;
	t_stack		stack_a;
	t_stack		stack_b;
	t_move		move;
	int			arg_index;
	int			bench_mode;

	if (argc < 2)
		return (1);
	arg_index = 1;
	ctx.a = &stack_a;
	ctx.b = &stack_b;
	ctx.move = &move;
	bench_mode = 0;
	while (arg_index < argc)
	{
		if (ft_strncmp(argv[arg_index], "--bench", 8) == 0)
		{
			bench_mode = 1;
			arg_index++;
			continue ;
		}
		if (parse_algorithm(argv[arg_index], &algorithm))
		{
			ctx.has_algorithm = 1;
			arg_index++;
			continue ;
		}
		break ;
	}
	init_context(&ctx);
	if ((ctx.has_algorithm && !argparser(argc, argv, ctx.a))
			|| (!ctx.has_algorithm && !argparser(argc, argv, ctx.a)))
	{
		stack_clear(ctx.a);
		stack_clear(ctx.b);
		clear_instruction_list(&ctx);
		write(2, "Error\n", 6);
		return (0);
	}
	if (ctx.has_algorithm)
		run_algorithm(algorithm, &ctx);
	else
		parser(&ctx);
	fflush(stdout);
	print_bench_instructions(&ctx);
	if (bench_mode)
		bench_print(&ctx);
	stack_clear(ctx.a);
	stack_clear(ctx.b);
	return (0);
}
