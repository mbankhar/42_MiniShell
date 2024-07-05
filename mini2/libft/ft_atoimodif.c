/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoimodif.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 09:54:57 by mbankhar          #+#    #+#             */
/*   Updated: 2024/05/04 17:22:00 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoimodif(char *str)
{
	int			i;
	int			sign;
	long int	res;

	res = 0;
	i = 0;
	sign = 1;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * sign);
}

	// if (str[i] != '\0' || (res * sign) > INT_MAX || (res * sign) < INT_MIN)
	// {
	// 	ft_putendl_fd("Error12", 2);
	// 	printf("%c", str[i]);
	// 	exit(EXIT_FAILURE);
	// }