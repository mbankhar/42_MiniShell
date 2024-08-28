/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:40:17 by mbankhar          #+#    #+#             */
/*   Updated: 2024/05/07 09:17:19 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start_string;
	size_t	end_string;
	char	*new_string;

	start_string = 0;
	end_string = 0;
	if (!s1)
		return (NULL);
	if (!*s1)
		return (ft_strdup(""));
	if (!set)
		return (ft_strdup(s1));
	end_string = ft_strlen(s1) - 1;
	while (s1[start_string] != '\0' && ft_strchr(set, s1[start_string]))
		start_string++;
	while (ft_strchr(set, s1[end_string]) && end_string > 0)
		end_string--;
	if (end_string < start_string)
		return (NULL);
	new_string = ft_substr(s1, start_string, end_string - start_string + 1);
	return (new_string);
}

// int main()
// {
// 	char string1[50] = "abcac";
// 	char string2[20] = "cba";
// 	printf("%s", ft_strtrim(0, ""));
// }