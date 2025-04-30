/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:32:08 by abostrom          #+#    #+#             */
/*   Updated: 2025/04/30 16:25:00 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdarg.h>

#include "ft_printf.h"

static size_t	ft_strlen(const char *string)
{
	size_t	length;

	length = 0;
	while (string != NULL && string[length] != '\0')
		length++;
	return (length);
}

static int	write_escape_char(va_list *args)
{
	const char	character = (char) va_arg(*args, int);

	return (write(1, &character, 1));
}

static int	write_escape_string(va_list *args)
{
	const char *const	string = va_arg(*args, const char*);

	if (string == NULL)
		return (write(1, "(null)", 6));
	return (write(1, string, ft_strlen(string)));
}

// FIXME: Can't use __int128 presumably.
static int	write_digits(__int128 value, const char *digits, int base)
{
	int	written;

	written = 1 + (value < 0);
	if (value < 0)
	{
		write(1, "-", 1);
		value = -value;
	}
	if (value >= base)
		written += write_digits(value / base, digits, base);
	write(1, &digits[value % base], 1);
	return (written);
}

static int	write_escape(char esc, va_list *args)
{
	unsigned long long	pointer;

	if (esc == '%')
		return (write(1, "%", 1));
	else if (esc == 'c')
		return (write_escape_char(args));
	else if (esc == 's')
		return (write_escape_string(args));
	else if (esc == 'd' || esc == 'i')
		return (write_digits(va_arg(*args, int), "0123456789", 10));
	else if (esc == 'u')
		return (write_digits(va_arg(*args, unsigned), "0123456789", 10));
	else if (esc == 'x')
		return (write_digits(va_arg(*args, unsigned), "0123456789abcdef", 16));
	else if (esc == 'X')
		return (write_digits(va_arg(*args, unsigned), "0123456789ABCDEF", 16));
	else if (esc == 'p')
	{
		pointer = va_arg(*args, unsigned long long);
		if (pointer == 0)
			return (write(1, "(nil)", 5));
		write(1, "0x", 2);
		return (2 + write_digits(pointer, "0123456789abcdef", 16));
	}
	return (0);
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
