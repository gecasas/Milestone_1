/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 16:11:13 by gecasas           #+#    #+#             */
/*   Updated: 2026/09/27 17:57:26 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(char const *format, ...)
{
	va_list	args;
	int		count;
	int		i;
	int		checker;

	va_start(args, format);
	count = 0;
	i = 0;
	while (format[i] != '\0')
	{
		if (format [i] == '%')
			checker = ft_format_selector(format[++i], &args);
		else
			checker = write (1, &format[i], 1);
		if (checker < 0)
		{
			va_end (args);
			return (-1);
		}
		count += checker;
		if (format[i] != '\0')
			i++;
	}
	va_end(args);
	return (count);
}

int	ft_format_selector(char c, va_list *args)
{
	if (c == 'c')
		return (ft_handle_c(args));
	if (c == 's')
		return (ft_handle_s(args));
	if (c == 'd' || c == 'i')
		return (ft_handle_d(args));
	if (c == 'u')
		return (ft_handle_u(args));
	if (c == 'x')
		return (ft_handle_x(args, 0));
	if (c == 'X')
		return (ft_handle_x(args, 1));
	if (c == 'p')
		return (ft_handle_p(args));
	if (c == '%')
		return (ft_handle_percent());
	return (0);
}
