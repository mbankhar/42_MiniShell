/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 09:29:03 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/09 15:19:53 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if ((char)c == '\0')
		return ((char *)&s[i]);
	else
		return (NULL);
}

// int main ()
// {
// 	const char str[] = "tripouille";
// 	const char ch = 1024;
// 	char *ret;
// 	char *ret2;
//    ret = ft_strchr(str, ch);
//    ret2 = strchr(str, ch);

//    printf("String after |%c| is - |%s|\n", ch, ret);
//    printf("String after |%c| is - |%s|\n", ch, ret2);

//    return(0);
// }
