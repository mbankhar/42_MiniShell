/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 10:16:34 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/08 15:27:00 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if ((c < 123 && c > 96) || (c < 91 && c > 64) || (c < 58 && c > 47))
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

// 	c = 'K';
// 	printf("%d\n", ft_isalnum(c));
// 	printf("%d\n", isalnum(c));
// }
