/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_memchr.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/10 16:54:12 by abarba-m         #+#    #+#              */
/*   Updated: 2026/09/24 22:44:52 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	const unsigned char	*p;

	p = (const unsigned char *) s;
	i = 0;
	while (i < n)
	{
		if (p[i] == c)
			return ((void *) & p[i]);
		i++;
	}
	return (NULL);
}
