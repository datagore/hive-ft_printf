/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:32:08 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/04 12:49:20 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>

#include "ft_printf.h"

#define DECIMAL "0123456789"
#define HEX_LOWERCASE "0123456789abcdef"
#define HEX_UPPERCASE "0123456789ABCDEF"

static int	write_char(char character)
{
	return (write(1, &character, 1));
}

static int	write_string(const char *string)
{
	int	length;

	if (string == NULL)
		string = "(null)";
	length = 0;
	while (string[length] != '\0')
		length++;
	return (write(1, string, length));
}

static int	write_uint(uintptr_t value, const char *digits, uintptr_t base)
{
	int	length;

	length = 0;
	if (value >= base)
		length = write_uint(value / base, digits, base);
	if (length < 0 || write_char(digits[value % base]) < 0)
		return (-1);
	return (length + 1);
}

static int	write_int(intptr_t signed_value, const char *digits, intptr_t base)
{
	uintptr_t	unsigned_value;
	int			length;

	unsigned_value = signed_value;
	if (signed_value < 0)
	{
		if (write_char('-') < 0)
			return (-1);
		unsigned_value = -signed_value;
	}
	length = write_uint(unsigned_value, digits, base);
	if (length < 0)
		return (-1);
	return (length + (signed_value < 0));
}

static int	write_pointer(uintptr_t pointer)
{
	int	length;

	if (pointer == 0)
		return (write_string("(nil)"));
	if (write_string("0x") < 0)
		return (-1);
	length = write_uint(pointer, HEX_LOWERCASE, 16);
	if (length < 0)
		return (-1);
	return (length + 2);
}

static int	write_conversion(char conversion, va_list *args)
{
	if (conversion == '\0')
		return (-1);
	if (conversion == '%')
		return (write_char('%'));
	if (conversion == 'c')
		return (write_char(va_arg(*args, int)));
	if (conversion == 's')
		return (write_string(va_arg(*args, const char *)));
	if (conversion == 'd' || conversion == 'i')
		return (write_int(va_arg(*args, int), DECIMAL, 10));
	if (conversion == 'u')
		return (write_uint(va_arg(*args, unsigned int), DECIMAL, 10));
	if (conversion == 'x')
		return (write_uint(va_arg(*args, unsigned int), HEX_LOWERCASE, 16));
	if (conversion == 'X')
		return (write_uint(va_arg(*args, unsigned int), HEX_UPPERCASE, 16));
	if (conversion == 'p')
		return (write_pointer(va_arg(*args, uintptr_t)));
	if (write_char('%') < 0 || write_char(conversion) < 0)
		return (-1);
	return (2);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		length;
	int		total;

	if (format == NULL)
		return (-1);
	va_start(args, format);
	total = 0;
	while (*format != '\0' && total >= 0)
	{
		if (*format == '%')
			length = write_conversion(*++format, &args);
		else
			length = write_char(*format);
		total += length;
		if (length < 0)
			total = -1;
		format++;
	}
	va_end(args);
	return (total);
}
