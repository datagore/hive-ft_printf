/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:33:08 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/09 10:19:36 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdint.h>

# define DECIMAL "0123456789"
# define HEX_LOWER "0123456789abcdef"
# define HEX_UPPER "0123456789ABCDEF"

int		ft_printf(const char *format, ...);
void	write_char(int *res, char character);
void	write_string(int *res, const char *string);
void	write_uint(int *res, const char *digits, uintptr_t base, uintptr_t num);
void	write_int(int *res, const char *digits, intptr_t base, intptr_t num);
void	write_pointer(int *res, void *pointer);

#endif
