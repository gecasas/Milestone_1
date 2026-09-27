/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 16:07:09 by gecasas           #+#    #+#             */
/*   Updated: 2026/09/27 17:58:07 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTF_H
# define PRINTF_H

# include "Libft/libft.h"
# include <stdarg.h>

int		ft_printf(char const *format, ...);
void	ft_putchar(char c);

# endif