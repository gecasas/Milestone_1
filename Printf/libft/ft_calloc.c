/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 15:56:08 by gecasas           #+#    #+#             */
/*   Updated: 2026/09/21 16:01:08 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Allocates memory for an array and initializes all bytes to zero. */

void	*ft_calloc(size_t count, size_t size)
{
	char	*str;
	size_t	total;

	total = count * size;
	if (count != 0 && (total / count) != size)
		return (NULL);
	str = malloc(total);
	if (!str)
		return (NULL);
	ft_bzero(str, size * count);
	return (str);
}
