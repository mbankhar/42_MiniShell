/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:17:45 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/11 18:17:23 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t		i;
	size_t		dst_len;
	size_t		src_len;

	i = 0;
	src_len = ft_strlen((char *) src);
	while (dst[i] && i < dstsize)
	{
		i++;
	}
	dst_len = i;
	if (dstsize <= dst_len)
		return (src_len + dstsize);
	i = 0;
	while (src[i] && i < dstsize - dst_len -1)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}

// int main()
// {
// 	char	a[100] = "pqrstuvwxyz";
// 	char	b[50] = "abcd";
// 	char	c[100] = "pqrstuvwxyz";
// 	char	d[50] = "abcd";
// 	ft_strlcat(a, b, 20);
// 	strlcat(c, d, 20);
// 	printf("%s\n", a);
// 	printf("%s\n", c);
// }
