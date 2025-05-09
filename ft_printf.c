/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:32:08 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/09 10:20:13 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <unistd.h>

#include "ft_printf.h"

static void	write_conversion(int *res, char type, va_list *arguments)
{
	if (type == '%')
		write_char(res, '%');
	else if (type == 'c')
		write_char(res, va_arg(*arguments, int));
	else if (type == 's')
		write_string(res, va_arg(*arguments, const char *));
	else if (type == 'i' || type == 'd')
		write_int(res, DECIMAL, 10, va_arg(*arguments, int));
	else if (type == 'u')
		write_uint(res, DECIMAL, 10, va_arg(*arguments, unsigned int));
	else if (type == 'x')
		write_uint(res, HEX_LOWER, 16, va_arg(*arguments, unsigned int));
	else if (type == 'X')
		write_uint(res, HEX_UPPER, 16, va_arg(*arguments, unsigned int));
	else if (type == 'p')
		write_pointer(res, va_arg(*arguments, void *));
	else
		*res = -1;
}

int	ft_printf(const char *format, ...)
{
	va_list		arguments;
	int			res;

	if (format == NULL)
		return (-1);
	res = 0;
	va_start(arguments, format);
	while (*format != '\0' && res >= 0)
	{
		if (*format == '%')
			write_conversion(&res, *++format, &arguments);
		else
			write_char(&res, *format);
		format++;
	}
	va_end(arguments);
	return (res);
}
