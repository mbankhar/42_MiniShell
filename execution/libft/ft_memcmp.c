/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:49:41 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/08 15:29:52 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t					i;
	const unsigned char		*temp;
	const unsigned char		*temptoo;

	temp = s1;
	temptoo = s2;
	i = 0;
	while (i < n && temp[i] == temptoo[i])
	{
		i++;
	}
	if (i == n)
	{
		return (0);
	}
	else
	{
		return (temp[i] - temptoo[i]);
	}
}

// int main()
// {
//     const char str1[] = "Hello, world!";
//     const char str2[] = "Hello, world!";
//     const char str3[] = "Goodbye, world!";

//     // Testing equal strings
//     int result1 = ft_memcmp(str1, str2, sizeof(str1));
//     printf("Comparison result 1: %d\n", result1); // Expecting 0

//     // Testing different strings
//     int result2 = ft_memcmp(str1, str3, sizeof(str1));
//     printf("Comparison result 2: %d\n", result2); // Expecting non-zero value

//     return 0;
// }
