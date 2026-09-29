/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ memcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 14:42:09 by gecasas           #+#    #+#             */
/*   Updated: 2026/09/09 14:42:09 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Copies n bytes from memory area src to memory area dst. */

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*strs;
	unsigned char	*strd;

	strs = (unsigned char *)src;
	strd = (unsigned char *)dst;
	i = 0;
	while (i < n)
	{
		strd[i] = strs[i];
		i++;
	}
	return (dst);
}
