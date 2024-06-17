/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 10:40:04 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/08 14:11:31 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c < 127 && c > 31)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

// int main()
// {
// 	char	c;

// 	c = 33;
// 	printf("%d\n", ft_isprint(c));
// 	printf("%d\n", isprint(c));
// }