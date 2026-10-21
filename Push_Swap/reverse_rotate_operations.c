/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   reverse_rotate_operations.c                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/13 13:52:21 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/21 15:44:39 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	rra(t_context *ctx)
{
	if (!ctx || !ctx->a || ctx->a->size < 2)
		return ;
	append_instruction(ctx, "rra");
	ctx->a->top = ctx->a->top->prev;
}

void	rrb(t_context *ctx)
{
	if (!ctx || !ctx->b || ctx->b->size < 2)
		return ;
	append_instruction(ctx, "rrb");
	ctx->b->top = ctx->b->top->prev;
}

void	rrr(t_context *ctx)
{
	if (!ctx || !ctx->a || ctx->a->size < 2 || !ctx->b || ctx->b->size < 2)
		return ;
	append_instruction(ctx, "rrr");
	ctx->a->top = ctx->a->top->prev;
	ctx->b->top = ctx->b->top->prev;
}
