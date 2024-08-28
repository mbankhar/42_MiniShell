/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:40:49 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/11 18:05:54 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	y;

	i = 0;
	if (*haystack == '\0' && *needle == '\0')
		return ((char *)haystack);
	if (*haystack == '\0')
		return (NULL);
	if (*needle == '\0')
		return ((char *)haystack);
	while (i < len && haystack[i])
	{
		y = 0;
		while (haystack[i + y] == needle[y]
			&& needle[y] != '\0' && i + y < len)
		{
			y++;
		}
		if (needle[y] == '\0')
		{
			return ((char *)&haystack[i]);
		}
		i++;
	}
	return (NULL);
}

// int main()
// {
// char *s1 = "abc";
//         char *s2 = "abcdef";
//         size_t bs = 5;
// 		printf("%smine\n", ft_strnstr(s1, s2, bs));
// 		printf("%s", strnstr(s1, s2, bs));

// }
