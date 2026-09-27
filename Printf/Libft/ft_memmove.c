/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 16:28:30 by gecasas           #+#    #+#             */
/*   Updated: 2026/09/09 16:28:30 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Copies n bytes from src to dst, safely handling overlapping memory. */

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			i;
	unsigned char	*strs;
	unsigned char	*strd;

	strs = (unsigned char *)src;
	strd = (unsigned char *)dst;
	if (strs > strd)
	{
		i = 0;
		while (i < len)
		{
			strd[i] = strs[i];
			i++;
		}
	}
	else if (strs < strd)
	{
		i = len;
		while (i > 0)
		{
			i--;
			strd[i] = strs[i];
		}
	}
	return (dst);
}
