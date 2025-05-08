/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 23:51:20 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/08 21:39:35 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "ft_printf.h"

void	write_char(int *res, char character)
{
	if (*res < 0)
		return ;
	*res += 1;
	if (write(1, &character, 1) < 1)
		*res = -1;
}

void	write_string(int *res, const char *string)
{
	if (!string)
		string = "(null)";
	while (*string != '\0')
		write_char(res, *string++);
}

void	write_uint(int *res, uintptr_t num, const char *digits, uintptr_t base)
{
	if (num >= base)
		write_uint(res, num / base, digits, base);
	write_char(res, digits[num % base]);
}

void	write_int(int *res, intptr_t num, const char *digits, intptr_t base)
{
	if (num >= 0)
		return (write_uint(res, num, digits, base));
	write_char(res, '-');
	write_uint(res, -num, digits, base);
}

void	write_pointer(int *res, void *pointer)
{
	if (!pointer)
		return (write_string(res, "(nil)"));
	write_string(res, "0x");
	write_uint(res, (uintptr_t) pointer, HEX_LOWER, 16);
}
