/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 13:25:52 by mbankhar          #+#    #+#             */
/*   Updated: 2024/04/30 16:04:28 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	copy_backward(char *dst, const char *src, size_t len)
{
	while (len > 0)
	{
		dst[len - 1] = src[len - 1];
		len--;
	}
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t		i;
	char		*temp_dest;
	const char	*temp_src;

	temp_dest = dst;
	temp_src = src;
	i = 0;
	if (!dst && !src)
		return (dst);
	if (temp_src < temp_dest)
		copy_backward(temp_dest, temp_src, len);
	else
	{
		while (i < len)
		{
			temp_dest[i] = temp_src[i];
			i++;
		}
	}
	return (dst);
}

// int main() 
// {
//     int str[100] = "12345"; 
//     char *first, *second; 
//     first = str; 
//     second = str; 
//     printf("Original string :%s\n ", str); 

//     // when overlap happens then it just ignore it 
//     memcpy(first + 8, first, 10); 
//     printf("memcpy overlap : %s\n ", str); 

//     // when overlap it start from first position 
//     memmove(second + 8, first, 10); 
//     printf("memmove overlap : %s\n ", str); 
// }
