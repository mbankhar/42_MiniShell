/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:11:53 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/09 16:27:59 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (0);
	while (1 < n && s1[i] == s2[i] && s1[i] != '\0')
	{
		n--;
		i++;
	}
	return ((unsigned char)(s1[i]) - (unsigned char)(s2[i]));
}

// int main()
// {
// 	char	str1[10] = "aa";
// 	char	str2[10] = "ab";
// 	printf("%d\n", ft_strncmp(str1, str2, 1));
// 	printf("%d\n", strncmp(str1, str2, 1));
// }