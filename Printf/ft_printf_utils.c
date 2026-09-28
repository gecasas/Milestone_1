/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 17:44:51 by gecasas           #+#    #+#             */
/*   Updated: 2026/09/27 17:46:08 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_handle_c(va_list *args)
{
	unsigned char	c;

	c = (unsigned char) va_arg(*args, int);
	return (write(1, &c, 1));
}

int	ft_handle_s(va_list *args)
{
	char	*str;

	str = va_arg(*args, char *);
	if (!str)
		return (write (1, "(null)", 6));
	return (write (1, str, ft_strlen(str)));
}

int	ft_handle_d(va_list *args)
{
	int		n;
	int		count;
	char	*str;

	n = (int) va_arg(*args, int);
	str = ft_itoa(n);
	if (!str)
		return (-1);
	count = write (1, str, ft_strlen(str));
	free (str);
	return (count);
}

int	ft_handle_u(va_list *args)
{
	char			*str;
	unsigned int	u;
	int				count;

	u = (unsigned int) va_arg(*args, unsigned int);
	str = ft_utoa(u);
	if (!str)
		return (-1);
	count = write (1, str, ft_strlen(str));
	free (str);
	return (count);
}

int	ft_handle_x(va_list *args, int upper)
{
	char			*str;
	unsigned int	x;
	int				count;

	x = (unsigned int) va_arg(*args, unsigned int);
	str = ft_xtoa(x, upper);
	if (!str)
		return (-1);
	count = write (1, str, ft_strlen(str));
	free (str);
	return (count);
}

