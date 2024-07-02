/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 10:41:49 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/08 15:25:24 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*temp;

	temp = s;
	i = 0;
	while (i < n)
	{
		temp[i] = '\0';
		i++;
	}
}

// int main()
// {
// 	char str[20] = "Test code";
// 	printf("%s\n", str);
// 	bzero(str + 6, 5 * sizeof(char));

// 	printf("%s\n", str);
// 		char strr[20] = "Test kode";
// 	printf("%s\n", strr);
// 	ft_bzero(strr + 6, 5 * sizeof(char));

// 	printf("%s\n", strr);
// }