/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_strtrim.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/23 20:19:48 by abarba-m         #+#    #+#              */
/*   Updated: 2026/09/24 23:01:22 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	fwd;
	size_t	bwd;

	fwd = 0;
	bwd = ft_strlen(s1);
	while (s1[fwd] && ft_strchr(set, s1[fwd]))
		fwd++;
	if (fwd == bwd)
		return (ft_substr(s1, fwd, 0));
	bwd--;
	while (bwd > fwd && ft_strchr(set, s1[bwd]))
		bwd--;
	return (ft_substr(s1, fwd, bwd - fwd + 1));
}
