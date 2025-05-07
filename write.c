/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 23:51:20 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/07 23:56:14 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	write_char(t_state *s, char chr)
{
	s->buffer[s->length++] = chr;
}

void	write_str(t_state *s, const char *str)
{
	if (!str)
		str = "(null)";
	s->output = str;
	while (str[s->length] != '\0')
		s->length++;
}

void	write_uint(t_state *s, uintptr_t n, const char *digits, uintptr_t base)
{
	if (n >= base)
		write_uint(s, n / base, digits, base);
	write_char(s, digits[n % base]);
}

void	write_int(t_state *s, intptr_t n, const char *digits, intptr_t base)
{
	if (n >= 0)
		return (write_uint(s, n, digits, base));
	write_char(s, '-');
	write_uint(s, -n, digits, base);
}

void	write_ptr(t_state *s, uintptr_t value)
{
	if (value == 0)
		return (write_str(s, "(nil)"));
	write_char(s, '0');
	write_char(s, 'x');
	write_uint(s, value, HEX_LOWER, 16);
}
