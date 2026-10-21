/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_printf.h                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/27 15:47:13 by abarba-m         #+#    #+#              */
/*   Updated: 2026/10/21 17:05:10 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

//Libraries
# include <stdarg.h>
# include "../libft.h"

//Functions
int	ft_printf(char const *input, ...);
int	ft_formatchecker(va_list args, char format);
int	ft_putchar(int c);
int	ft_putstr(char *s);
int	ft_putnbr(int n);
int	ft_putptr(void *ptr);
int	ft_puthex(unsigned long n, char format);
int	ft_putunbr(unsigned int n);

#endif
