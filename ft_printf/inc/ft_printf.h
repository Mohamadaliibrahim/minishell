/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 12:08:59 by mohamibr          #+#    #+#             */
/*   Updated: 2024/12/26 11:06:40 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "../../inc/minishell.h"
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

int		ft_printf(const char *hello, ...);
char	*ft_itoa(int n);
char	*ft_strdup(const char *src);
char	*ft_utoa(unsigned int n);
int		ft_strlen1(const char *str);
void	ft_putnbr_fd(int n, int fd);
int		ft_putchar_fd1(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	check_1(const char ch, va_list ap, int *count);
void	check_2(const char ch, va_list ap, int *count);
void	check_arg(const char ch, va_list ap, int *count);
void	hexa_l(char *str, unsigned int x);
void	hexa(char *str, unsigned long x, int z);
void	hexa_u(char *str, unsigned int x);
void	pointer_p(char *str, void *p);

#endif
