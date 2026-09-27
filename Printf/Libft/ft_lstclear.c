/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 15:08:37 by gecasas           #+#    #+#             */
/*   Updated: 2026/09/25 16:20:07 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Deletes and frees the given node and every successor of that node. */

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;
	t_list	*cont;

	if (!lst || !del)
		return ;
	cont = *lst;
	while (cont != NULL)
	{
		tmp = cont->next;
		ft_lstdelone(cont, del);
		cont = tmp;
	}
	*lst = NULL;
}
