/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   flags.c                                           :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/21 15:00:43 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/22 21:03:00 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"
#include <stdio.h>

void	append_instruction(t_context *ctx, char *op)
{
	t_list	*node;
	char	*copy;

	if (!ctx || !op)
		return ;
	copy = ft_strdup(op);
	if (!copy)
		return ;
	node = ft_lstnew(copy);
	if (!node)
	{
		free(copy);
		return ;
	}
	if (!ctx->instructions)
		ctx->instructions = node;
	else
		ft_lstadd_back(&ctx->instructions, node);
	ctx->instructions_tail = ft_lstlast(ctx->instructions);
}

void	print_instructions(t_context *ctx)
{
	t_list	*current;
	int		i;

	if (!ctx || !ctx->instructions)
		return ;
	current = ctx->instructions;
	i = 0;
	while (current)
	{
		i++;
		current = current->next;
	}
	ctx->total_ops = i;
	if (ctx->bench_mode)
	{
		ft_putnbr_fd(i, 1);
		write(1, "\n", 1);
	}
	current = ctx->instructions;
	while (current)
	{
		write(1, current->content, ft_strlen((char *) current->content));
		write(1, "\n", 1);
		current = current->next;
	}
}

void	clear_instr_list(t_context *ctx)
{
	if (!ctx)
		return ;
	ft_lstclear(&ctx->instructions, free);
	ctx->instructions = NULL;
	ctx->instructions_tail = NULL;
}
