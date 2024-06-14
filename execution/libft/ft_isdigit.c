/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 10:03:39 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/08 15:27:39 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c < 58 && c > 47)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

// int	main()
// {
// 	char	c;

// 	c = 'a';

// 	printf("%d\n", ft_isdigit(c));
// 	printf("%d", isdigit(c));
// }