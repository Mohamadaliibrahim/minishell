/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 12:46:30 by mmachlou          #+#    #+#             */
/*   Updated: 2024/09/21 13:45:15 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int result = 0;
	int sign = 1;

	// Skip whitespace characters
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;

	// Handle the sign
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}

	// Convert digits to integer
	while (*str >= '0' && *str <= '9')
	{
		// Handle overflow/underflow
		if (result > (2147483647 - (*str - '0')) / 10)
		{
			if (sign == 1)
				return (2147483647);  // INT_MAX
			else
				return (-2147483648); // INT_MIN
		}
		result = result * 10 + (*str - '0');
		str++;
	}

	return (sign * result);
}
