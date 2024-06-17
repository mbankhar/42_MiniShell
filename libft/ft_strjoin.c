/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 13:04:24 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/11 16:19:07 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*new_string;
	size_t	i;
	size_t	y;

	i = 0;
	y = 0;
	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen((char *)s1);
	len_s2 = ft_strlen((char *)s2);
	new_string = (char *)malloc((len_s1 + len_s2 +1) * sizeof(char));
	if (new_string == NULL)
		return (NULL);
	while (i < len_s1)
	{
		new_string[i] = s1[i];
		i++;
	}
	while (y < len_s2)
		new_string[i++] = s2[y++];
	new_string[i] = '\0';
	return (new_string);
}

// int main()
// {
// 	char *string1 = "Hello";
// 	char *string2 = " World";
// 	printf("%s", ft_strjoin(string1, string2));
// }