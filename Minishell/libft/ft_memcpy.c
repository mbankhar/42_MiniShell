/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 10:57:24 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/09 10:54:39 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t		i;
	char		*temp;
	const char	*temptoo;

	temptoo = src;
	temp = dst;
	i = 0;
	if (!dst && !src)
		return (dst);
	while (i < n)
	{
		temp[i] = temptoo[i];
		i++;
	}
	return (dst);
}

// int main() 
// { 
// 	char str[100] = "0"; 
// 	char str2[50] = "0"; 
// 	printf("Original string :%s\n ", str); 

// 	// when overlap happens then it just ignore it 
// 	memcpy(str, str2, 3); 
// 	printf("memcpy overlap : %s\n ", str); 

// 	// when overlap it start from first position 
// 	ft_memcpy(str, str2, 3); 
// 	printf("ft_memcpy overlap : %s\n ", str); 

// 	return 0; 
// }
