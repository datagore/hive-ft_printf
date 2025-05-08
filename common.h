/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:33:08 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/08 10:21:42 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
# define COMMON_H

# include <stdint.h>

# define DECIMAL "0123456789"
# define HEX_LOWER "0123456789abcdef"
# define HEX_UPPER "0123456789ABCDEF"

typedef struct s_state
{
	char	buffer[20];
	int		length;
	char	*output;
}	t_state;

int		ft_printf(const char *format, ...);
void	write_char(t_state *s, char chr);
void	write_str(t_state *s, const char *str);
void	write_uint(t_state *s, uintptr_t n, const char *digits, uintptr_t base);
void	write_int(t_state *s, intptr_t n, const char *digits, intptr_t base);
void	write_ptr(t_state *s, void *ptr);

#endif
