/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_atoi.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/21 14:18:49 by abarba-m         #+#    #+#              */
/*   Updated: 2026/09/24 22:21:33 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	aux;
	int	nbr;

	i = 0;
	aux = 1;
	nbr = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-')
	{
		aux *= -1;
		i++;
	}
	if (nptr[i] == '+')
		i++;
	while (ft_isdigit(nptr[i]))
	{
		nbr *= 10;
		nbr += nptr[i] - '0';
		i++;
	}
	return (aux * nbr);
}
