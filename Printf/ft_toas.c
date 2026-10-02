/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toas.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 12:45:18 by gecasas           #+#    #+#             */
/*   Updated: 2026/09/30 18:10:16 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_utoa(unsigned int u)
{
	char			*str;
	int				count;
	unsigned int	tmp;

	tmp = u;
	count = 0;
	if (tmp == 0)
		count = 1;
	while (tmp != 0)
	{
		tmp = tmp / 10;
		count++;
	}
	str = malloc(sizeof(char) * (count + 1));
	if (!str)
		return (NULL);
	str[count] = '\0';
	while (count > 0)
	{
		count--;
		str[count] = (u % 10) + '0';
		u = u / 10;
	}
	return (str);
}

char	*ft_xtoa(unsigned long x, int upper)
{
	char			*str;
	char			*hex;
	int				count;

	if (upper == 1)
		hex = "0123456789ABCDEF";
	else
		hex = "0123456789abcdef";
	count = ft_hexlen(x);
	str = malloc(sizeof(char) * (count + 1));
	if (!str)
		return (NULL);
	str[count] = '\0';
	while (count > 0)
	{
		count--;
		str[count] = hex[x % 16];
		x = x / 16;
	}
	return (str);
}

int	ft_hexlen(unsigned long x)
{
	int	count;

	count = 1;
	while (x >= 16)
	{
		x = x / 16;
		count++;
	}
	return (count);
}

int	ft_handle_percent(void)
{
	return (write (1, "%", 1));
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
