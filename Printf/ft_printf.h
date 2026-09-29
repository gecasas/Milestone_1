/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 16:07:09 by gecasas           #+#    #+#             */
/*   Updated: 2026/09/27 18:01:00 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft/libft.h"
# include <stdarg.h>

char	*ft_utoa(unsigned int u);
char	*ft_xtoa(unsigned long x, int upper);
int		ft_printf(char const *format, ...);
int		ft_hexlen(unsigned long x);
int		ft_handle_c(va_list *args);
int		ft_handle_s(va_list *args);
int		ft_handle_d(va_list *args);
int		ft_handle_u(va_list *args);
int		ft_handle_x(va_list *args, int upper);
int		ft_handle_p(va_list *args);
int		ft_handle_percent(void);
int		ft_format_selector(char c, va_list *args);

#endif