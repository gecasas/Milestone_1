/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 18:53:50 by gecasas           #+#    #+#             */
/*   Updated: 2026/09/23 19:53:40 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Allocates and returns a string representing the given integer. */

static int	ft_count(long n);

char	*ft_itoa(int n)
{
	long	nb;
	int		i;
	char	*str;
	int		len;

	nb = n;
	len = ft_count(nb);
	str = malloc (sizeof (char) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	if (nb < 0)
	{
		nb = nb * (-1);
		str[0] = '-';
		i++;
	}
	str[len] = '\0';
	while (len > i)
	{
		len--;
		str[len] = ((nb % 10) + '0');
		nb = nb / 10;
	}
	return (str);
}

static int	ft_count(long n)
{
	int	len;

	len = 0;
	if (n < 0)
	{
		len++;
		n = n * (-1);
	}
	else if (n == 0)
		return (1);
	while (n > 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

/*#include <stdio.h>

int	main(void)
{
	int	n = 12312;
	printf("%s", ft_itoa(n));
	return 0;
}*/