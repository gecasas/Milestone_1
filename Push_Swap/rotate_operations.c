/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   rotate_operations.c                               :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/12 15:40:33 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/21 15:46:04 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ra(t_context *ctx)
{
	if (!ctx || !ctx->a || ctx->a->size < 2)
		return ;
	append_instruction(ctx, "ra");
	ctx->a->top = ctx->a->top->next;
}

void	rb(t_context *ctx)
{
	if (!ctx || !ctx->b || ctx->b->size < 2)
		return ;
	append_instruction(ctx, "rb");
	ctx->b->top = ctx->b->top->next;
}

void	rr(t_context *ctx)
{
	if (!ctx || !ctx->a || ctx->a->size < 2 || !ctx->b || ctx->b->size < 2)
		return ;
	append_instruction(ctx, "rr");
	ctx->a->top = ctx->a->top->next;
	ctx->b->top = ctx->b->top->next;
}
