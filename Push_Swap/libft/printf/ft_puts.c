/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_puts.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/27 16:39:58 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/21 17:11:47 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(int c)
{
	write(2, &c, 1);
	return (1);
}

int	ft_putstr(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (write(2, "(null)", 6));
	while (s[i])
	{
		write(2, &s[i], 1);
		i++;
	}
	return (i);
}

int	ft_putnbr(int n)
{
	int	len;

	len = 0;
	if (n == -2147483648)
	{
		write(2, "-2147483648", 11);
		return (11);
	}
	if (n < 0)
	{
		write(2, "-", 1);
		n = -n;
		len++;
	}
	if (n >= 10)
		len += ft_putnbr(n / 10);
	write(2, &"0123456789"[n % 10], 1);
	len++;
	return (len);
}
