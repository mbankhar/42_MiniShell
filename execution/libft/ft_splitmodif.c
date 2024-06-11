/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitmodif.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:11:23 by mbankhar          #+#    #+#             */
/*   Updated: 2024/05/06 14:26:03 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlenmodif(const char *c, int i)
{
	int	counter;

	counter = 0;
	while (c[i] != '\0')
	{
		i++;
		counter++;
	}
	return (counter);
}

static size_t	ft_strlccpy(char *dst, const char *src, size_t dstsize)
{
	size_t		y;
	size_t		i;
	size_t		src_len;

	y = 0;
	i = 0;
	while (src[y] != '\0')
	{
		y++;
	}
	src_len = y;
	if (dstsize == 0)
		return (src_len);
	while (i < dstsize -1 && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	if (dstsize != 0)
		dst[i] = '\0';
	return (src_len);
}

static size_t	ft_lens(const char *c, char b)
{
	size_t	i;

	i = 0;
	while (*c != '\0' && *c != b)
	{
		c++;
		i++;
	}
	return (i);
}

static int	ft_malloc_check(char **arr, size_t i)
{
	int	temp;

	if (arr[i] == NULL)
	{
		temp = i;
		while (--temp >= 0)
			free(arr[temp]);
		free(arr);
		return (0);
	}
	return (1);
}

char	**ft_splitmodif(char const *s, char c)
{
	char	**arr;
	size_t	p;
	int		pos;

	if (ft_strchrmodif(s) != -1)
		pos = ft_strchrmodif(s) + 1;
	else if (ft_strchrmodif(s) != -1)
		pos = ft_strchrmodif(s) + 1;
	p = 0;
	arr = (char **)malloc(sizeof(char *) * (3));
	if (arr == NULL)
		return (NULL);
	while (s[p] == c)
		p++;
	arr[0] = (char *)malloc(sizeof(char) * (ft_lens(&s[p], c) + 1));
	if (!ft_malloc_check(arr, 0))
		return (NULL);
	ft_strlccpy(arr[0], &s[p], ft_lens(&s[p], c) + 1);
	arr[1] = (char *)malloc(sizeof(char) * (ft_strlenmodif(s, pos) - 1));
	if (!ft_malloc_check(arr, 1))
		return (NULL);
	ft_strlccpy(arr[1], &s[pos], ft_strlenmodif(s, pos));
	arr[2] = NULL;
	return (arr);
}
