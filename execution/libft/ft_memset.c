/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 09:38:29 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/08 15:30:58 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	*temp;

	temp = b;
	i = 0;
	while (i < len)
	{
		temp[i] = (unsigned char) c;
		i++;
	}
	return (b);
}

// int main()
// {
// 	char str[20] = "Test code";
// 	printf("%s\n", str);
// 	memset(str + 6, 'a', 5 * sizeof(char));
// 	printf("%s\n", str);
// 		char strr[20] = "Test kode";
// 	printf("%s\n", strr);
// 	ft_memset(strr + 6, 'a', 5 * sizeof(char));
// 	printf("%s", strr);
// }