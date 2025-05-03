/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:32:08 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/04 00:00:29 by abostrom         ###   ########.fr       */
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
	int	count;

	if (string == NULL)
		string = "(null)";
	count = 0;
	while (string[count] != '\0')
		count++;
	return (write(1, string, count));
}

static int	write_uint(uintptr_t value, const char *digits, uintptr_t base)
{
	int	count;

	count = 0;
	if (value >= base)
		count = write_uint(value / base, digits, base);
	if (count < 0 || write_char(digits[value % base]) < 0)
		return (-1);
	return (1 + count);
}

static int	write_int(intptr_t value, const char *digits, intptr_t base)
{
	uintptr_t	abs;
	int			count;

	abs = value;
	if (value < 0)
	{
		if (write_char('-') < 0)
			return (-1);
		abs = -value;
	}
	count = write_uint(abs, digits, base);
	if (count < 0)
		return (-1);
	return (count + (value < 0));
}

static int	write_pointer(uintptr_t pointer)
{
	int	count;

	if (pointer == 0)
		return (write_string("(nil)"));
	if (write_string("0x") < 0)
		return (-1);
	count = write_uint(pointer, HEX_LOWERCASE, 16);
	if (count < 0)
		return (-1);
	return (count + 2);
}

static int	write_escape(char esc, va_list *args)
{
	if (esc == '\0')
		return (-1);
	if (esc == '%')
		return (write_char('%'));
	if (esc == 'c')
		return (write_char(va_arg(*args, int)));
	if (esc == 's')
		return (write_string(va_arg(*args, const char *)));
	if (esc == 'd' || esc == 'i')
		return (write_int(va_arg(*args, int), DECIMAL, 10));
	if (esc == 'u')
		return (write_int(va_arg(*args, unsigned int), DECIMAL, 10));
	if (esc == 'x')
		return (write_int(va_arg(*args, unsigned int), HEX_LOWERCASE, 16));
	if (esc == 'X')
		return (write_int(va_arg(*args, unsigned int), HEX_UPPERCASE, 16));
	if (esc == 'p')
		return (write_pointer(va_arg(*args, uintptr_t)));
	if (write_char('%') < 0 || write_char(esc) < 0)
		return (-1);
	return (2);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count;
	int		total;

	if (format == NULL)
		return (-1);
	va_start(args, format);
	total = 0;
	while (*format && total >= 0)
	{
		if (*format == '%')
			count = write_escape(*++format, &args);
		else
			count = write_char(*format);
		total += count;
		if (count < 0)
			total = -1;
		format++;
	}
	va_end(args);
	return (total);
}
