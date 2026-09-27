/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:57:02 by gecasas           #+#    #+#             */
/*   Updated: 2026/09/09 15:57:02 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Sets the first n bytes of the memory area to zero. */

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, '\0', n);
}
