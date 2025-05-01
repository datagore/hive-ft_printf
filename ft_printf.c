/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:32:08 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/01 13:56:48 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>

#include "ft_printf.h"

static size_t	ft_strlen(const char *string)
{
	size_t	length;

	length = 0;
	while (string != NULL && string[length] != '\0')
		length++;
	return (length);
}

static int	write_char(char character)
{
	return (write(1, &character, 1));
}

static int	write_string(const char *string)
{
	if (string == NULL)
		string = "(null)";
	return (write(1, string, ft_strlen(string)));
}

static int	write_uint(uintptr_t value, const char *digits, uintptr_t base)
{
	int	written;

	written = 1;
	if (value >= base)
		written += write_uint(value / base, digits, base);
	write(1, &digits[value % base], 1);
	return (written);
}

static int	write_int(intptr_t value, const char *digits, intptr_t base)
{
	int	written;

	written = 0;
	if (value < 0)
	{
		write(1, "-", 1);
		value = -value;
		written++;
	}
	return (written + write_uint(value, digits, base));
}

static int	write_pointer(uintptr_t pointer)
{
	if (pointer == 0)
		return (write_string("(nil)"));
	return (write_string("0x") + write_uint(pointer, "0123456789abcdef", 16));
}

static int	write_escape(char esc, va_list *args)
{
	if (esc == '%')
		return (write(1, "%", 1));
	if (esc == 'c')
		return (write_char(va_arg(*args, int)));
	if (esc == 's')
		return (write_string(va_arg(*args, const char *)));
	if (esc == 'd' || esc == 'i')
		return (write_int(va_arg(*args, int), "0123456789", 10));
	if (esc == 'u')
		return (write_int(va_arg(*args, unsigned), "0123456789", 10));
	if (esc == 'x')
		return (write_int(va_arg(*args, unsigned), "0123456789abcdef", 16));
	if (esc == 'X')
		return (write_int(va_arg(*args, unsigned), "0123456789ABCDEF", 16));
	if (esc == 'p')
		return (write_pointer(va_arg(*args, uintptr_t)));
	return (write_char('%') + write_char(esc));
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		written;

	if (format == NULL)
		return (-1);
	written = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format++ == '%')
			written += write_escape(*format++, &args);
		else
			written += write(1, format - 1, 1);
	}
	va_end(args);
	return (written);
}
