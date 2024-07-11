/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 11:15:59 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/11 11:16:34 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	int		y;
	char	*substring;

	y = 0;
	if (start < ft_strlen(s))
	{
		if (ft_strlen(&s[start]) < len)
			len = ft_strlen(&s[start]);
	}
	else
	{
		len = 0;
		start = 0;
	}
	if (ft_strlen((char *)s) < (size_t)start)
		return (NULL);
	substring = (char *)malloc(len + 1 * sizeof(char));
	if (substring == NULL)
		return (NULL);
	ft_strlcpy(&substring[y], &s[start], len + 1);
	return (substring);
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// int main()
// {
// 	char	*new_string;
// 	char	string[40] = "1234";

// 	new_string = ft_substr(string, 10, 10);
// 	printf("%s", new_string);
// 	free(new_string);
// }