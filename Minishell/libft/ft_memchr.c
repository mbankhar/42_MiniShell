/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:31:05 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/11 17:50:06 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*temp;

	temp = (unsigned char *)s;
	i = 0;
	c = (unsigned char)c;
	while (i < n)
	{
		if (temp[i] == c)
			return ((void *)&temp[i]);
		i++;
	}
	return (NULL);
}

// int main ()
// {
// 	char	str1[20] = "Hel1lo";
// 	int		c = '1';
// 	printf("%s", ft_memchr(str1, c, 3));
// }
