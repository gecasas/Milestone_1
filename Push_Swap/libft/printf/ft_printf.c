/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_printf.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/27 15:48:37 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/22 21:35:01 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(char const *input, ...)
{
	va_list	args;
	int		i;
	int		len;

	i = 0;
	len = 0;
	if (!input)
		return (-1);
	va_start(args, input);
	while (input[i])
	{
		if (input[i] == '%')
		{
			i++;
			len += ft_formatchecker(args, input[i]);
		}
		else
			len += ft_putchar(input[i]);
		if (input[i] != '\0')
			i++;
	}
	va_end(args);
	return (len);
}

int	ft_formatchecker(va_list args, char format)
{
	if (format == 'c')
		return (ft_putchar(va_arg(args, int)));
	else if (format == 's')
		return (ft_putstr(va_arg(args, char *)));
	else if (format == 'p')
		return (ft_putptr(va_arg(args, void *)));
	else if (format == 'd' || format == 'i')
		return (ft_putnbr(va_arg(args, int)));
	else if (format == 'u')
		return (ft_putunbr(va_arg(args, unsigned int)));
	else if (format == 'x' || format == 'X')
		return (ft_puthex(va_arg(args, unsigned int), format));
	else if (format == 'f')
		return (ft_putfloat(va_arg(args, double)));
	else if (format == '%')
		return (ft_putchar(format));
	return (0);
}
