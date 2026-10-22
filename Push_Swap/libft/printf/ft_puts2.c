/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_puts2.c                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/27 17:00:55 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/22 21:37:04 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putptr(void *ptr)
{
	unsigned long long	address;
	int					len;

	if (ptr == NULL)
		return (write(2, "(nil)", 5));
	address = (unsigned long long) ptr;
	len = write(2, "0x", 2);
	len += ft_puthex(address, 'x');
	return (len);
}

int	ft_puthex(unsigned long n, char format)
{
	int	len;

	len = 0;
	if (n >= 16)
		len += ft_puthex(n / 16, format);
	if (format == 'x')
		len += write(2, &"0123456789abcdef"[n % 16], 1);
	else if (format == 'X')
		len += write(2, &"0123456789ABCDEF"[n % 16], 1);
	return (len);
}

int	ft_putunbr(unsigned int n)
{
	int	len;

	len = 0;
	if (n >= 10)
		len += ft_putunbr(n / 10);
	write(2, &"0123456789"[n % 10], 1);
	len++;
	return (len);
}

static int	ft_putull(unsigned long long n)
{
	int	len;

	len = 0;
	if (n >= 10)
		len += ft_putull(n / 10);
	len += write(2, &"0123456789"[n % 10], 1);
	return (len);
}

int	ft_putfloat(double n)
{
	unsigned long long	whole;
	unsigned int		fraction;
	int					len;

	len = 0;
	if (n < 0)
	{
		return (-1);
	}
	whole = (unsigned long long) n;
	fraction = (unsigned int)((n - whole) * 100 + 0.5);
	if (fraction == 100)
	{
		whole++;
		fraction = 0;
	}
	len += ft_putull(whole);
	len += write(1, ".", 1);
	if (fraction < 10)
		len += write(1, "0", 1);
	len += ft_putunbr(fraction);
	return (len);
}
