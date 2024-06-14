/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:25:42 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/11 10:01:03 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*pointer;

	pointer = malloc(count * size);
	if (pointer == NULL)
		return (NULL);
	ft_bzero(pointer, count * size);
	return (pointer);
}

// #include <stdio.h>
// #include <stdlib.h>

// // Declaration of ft_calloc function
// void *ft_calloc(size_t count, size_t size);

// int main()
// {
//     // Test parameters
//     size_t num_elements = 5;
//     size_t element_size = sizeof(int);

//     // Allocate memory using ft_calloc
//     int *arr = (int *)ft_calloc(num_elements, element_size);
//     int *arr2 = (int *)calloc(num_elements, element_size);

//     if (arr == NULL)
//     {
//         printf("Memory allocation failed.\n");
//         return 1;
//     }

//     // Print the allocated array
//     printf("Allocated array:\n");
//     for (size_t i = 0; i < num_elements; i++)
//     {
//         printf("%d ", arr[i]);
//     }
//     printf("\n");
//     for (size_t i = 0; i < num_elements; i++)
//     {
//         printf("%d ", arr2[i]);
//     }
//     printf("\n");

//     // Free allocated memory
//     free(arr);
//     free(arr2);

//     return 0;
// }
