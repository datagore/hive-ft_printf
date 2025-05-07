/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:32:08 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/07 23:52:19 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <unistd.h>

#include "ft_printf.h"

void	write_arg(t_state *s, char type, va_list *ap)
{
	if (type == '%')
		write_char(s, '%');
	else if (type == 'c')
		write_char(s, va_arg(*ap, int));
	else if (type == 's')
		write_str(s, va_arg(*ap, const char *));
	else if (type == 'd' || type == 'i')
		write_int(s, va_arg(*ap, int), DECIMAL, 10);
	else if (type == 'u')
		write_uint(s, va_arg(*ap, unsigned int), DECIMAL, 10);
	else if (type == 'x')
		write_uint(s, va_arg(*ap, unsigned int), HEX_LOWER, 16);
	else if (type == 'X')
		write_uint(s, va_arg(*ap, unsigned int), HEX_UPPER, 16);
	else if (type == 'p')
		write_ptr(s, va_arg(*ap, uintptr_t));
	else if (type != '\0')
	{
		write_char(s, '%');
		write_char(s, type);
	}
}

int	ft_printf(const char *format, ...)
{
	t_state	s;
	va_list	ap;
	int		result;

	if (format == NULL)
		return (-1);
	va_start(ap, format);
	result = 0;
	while (*format != '\0' && result >= 0)
	{
		s.length = 0;
		s.output = s.buffer;
		if (*format == '%')
			write_arg(&s, *++format, &ap);
		else
			write_char(&s, *format);
		result += s.length;
		if (write(1, s.output, s.length) < 0)
			result = -1;
		format++;
	}
	va_end(ap);
	return (result);
}
