/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   push_swap.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/06 11:50:22 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/22 14:52:56 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	bench_print(t_context *ctx)
{
	if (!ctx)
		return ;
	if (ctx->a)
		ft_printf("[Bench] disorder: %d\n", compute_disorder(ctx->a));
	else
		ft_printf("[Bench] disorder: N/A (stack a is NULL)\n");
	ft_printf("[Bench] strategy: %d\n", ctx->algorithm);
	ft_printf("[Bench] total_ops: %d\n", ctx->total_ops);
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
	ctx->algorithm = ALGO_ADAPTIVE;
	ctx->has_algorithm = 0;
}

void	init_argc_argv(int argc, char **argv, t_context *ctx, int *bench)
{
	int	arg_index;

	arg_index = 1;
	ctx->has_algorithm = 0;
	while (arg_index < argc)
	{
		if (ft_strncmp(argv[arg_index], "--bench", 8) == 0)
		{
			*bench = 1;
			arg_index++;
			continue ;
		}
		if (parse_algorithm(argv[arg_index], &ctx->algorithm))
		{
			ctx->has_algorithm = 1;
			arg_index++;
			continue ;
		}
		break ;
	}
}

void	global_clear(t_context ctx)
{
	stack_clear(ctx.a);
	stack_clear(ctx.b);
	clear_instr_list(&ctx);
}

int	main(int argc, char **argv)
{
	t_context	ctx;
	t_stack		stack_a;
	t_stack		stack_b;
	t_move		move;

	if (argc < 2)
		return (1);
	ctx.a = &stack_a;
	ctx.b = &stack_b;
	ctx.move = &move;
	init_context(&ctx);
	init_argc_argv(argc, argv, &ctx, &ctx.bench_mode);
	if (!argparser(argc, argv, ctx.a))
		return (global_clear(ctx), write(2, "Error\n", 6), 1);
	if (ctx.has_algorithm)
		run_algorithm(ctx.algorithm, &ctx);
	else
		parser(&ctx);
	print_instructions(&ctx);
	if (ctx.bench_mode)
		bench_print(&ctx);
	return (global_clear(ctx), 0);
}
