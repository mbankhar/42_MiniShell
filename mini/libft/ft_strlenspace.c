/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlenspace.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 11:54:21 by mbankhar          #+#    #+#             */
/*   Updated: 2024/05/06 12:35:19 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// I modified it so it doesnt count the spaces.

int	whitesoace(char c)
{
	if (c == ' ' || c == '\v' || c == '\t')
		return (0);
	return (1);
}

int	ft_strlenspace(char *c)
{
	int16_t	i;
	int		y;

	i = 0;
	y = 0;
	while (c[i] != '\0')
	{
		while (c[i] == whitesoace(c[i]))
			i++;
		if (c[i] <= '9' && c[i] >= '0')
			y++;
		while (c[i] <= '9' && c[i] >= '0')
			i++;
		i++;
	}
	return (y);
}
// int main()
// {
// 	char a[100] = "1    10    0 -1 -1 0 1 1 0 0 ";
// 	char b[100] = "1  2 30 -1  0  0  0  0  0  0    0  0  0  0    ";
// 	printf("%d\n", ft_strlenmodif(a));
// 	printf("%d\n", ft_strlenmodif(b));
// }
