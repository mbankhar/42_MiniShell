/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 10:11:30 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/11 11:10:25 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	int		strlen;
	char	*pointer;

	i = 0;
	strlen = ft_strlen((char *) s1);
	pointer = (char *)malloc((strlen + 1) * sizeof(char));
	if (pointer == NULL)
		return (NULL);
	while (s1[i])
	{
		pointer[i] = s1[i];
		i++;
	}
	pointer[i] = '\0';
	return (pointer);
}

// #include <stdio.h>
// #include <stdlib.h>

// int main() {
//     const char *original = "Hello, world!";
//     char *duplicate = ft_strdup(original);

//     if (duplicate == NULL) {
//         printf("Memory allocation failed.\n");
//         return 1;
//     }

//     printf("Original string: %s\n", original);
//     printf("Duplicate string: %s\n", duplicate);

//     // Remember to free the allocated memory
//     free(duplicate);

//     return 0;
// }
