/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_strrchr.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/10 16:28:27 by abarba-m         #+#    #+#              */
/*   Updated: 2026/09/25 00:52:44 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*last;

	i = 0;
	last = NULL;
	while (s[i] != '\0')
	{
		if ((unsigned char) s[i] == (unsigned char) c)
			last = (char *) & s[i];
		i++;
	}
	if ((unsigned char) c == '\0')
		return ((char *) & s[i]);
	return (last);
}
