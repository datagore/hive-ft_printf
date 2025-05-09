/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 23:51:20 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/09 10:22:16 by abostrom         ###   ########.fr       */
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

void	write_uint(int *res, const char *digits, uintptr_t base, uintptr_t num)
{
	if (num >= base)
		write_uint(res, digits, base, num / base);
	write_char(res, digits[num % base]);
}

void	write_int(int *res, const char *digits, intptr_t base, intptr_t num)
{
	if (num >= 0)
		return (write_uint(res, digits, base, num));
	write_char(res, '-');
	write_uint(res, digits, base, -num);
}

void	write_pointer(int *res, void *pointer)
{
	if (!pointer)
		return (write_string(res, "(nil)"));
	write_string(res, "0x");
	write_uint(res, HEX_LOWER, 16, (uintptr_t) pointer);
}
