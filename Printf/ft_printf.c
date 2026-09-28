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

	va_start(args, format);
	count = 0;
	i = 0;
	while (format[i] != '\0')
	{
		if (format [i] == '%')
		{
			count += ft_format_selector(&format[i + 1], args);
		}
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

int	ft_handle_p(va_list *args)
{
	char			*str;
	char			*res;
	int				count;
	unsigned long	p;

	p = (unsigned long)va_arg(*args, void *);
	if (!p)
		return (write (1, "(nil)", 5));
	str = ft_xtoa(p, 0);
	if (!str)
		return (-1);
	res = ft_strjoin("0x", str);
	if (!res)
	{
		free (str);
		return (-1);
	}
	count = write (1, res, ft_strlen(res));
	free (str);
	free (res);
	return (count);
}
