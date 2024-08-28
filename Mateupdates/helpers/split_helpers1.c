/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_helpers1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:15:10 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/27 17:59:00 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	ft_strlccpy(char *dst, const char *src, size_t dstsize)
{
	size_t	y;
	size_t	i;
	size_t	src_len;

	y = 0;
	i = 0;
	while (src[y] != '\0')
		y++;
	src_len = y;
	if (dstsize == 0)
		return (src_len);
	while (i < dstsize - 1 && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	if (dstsize != 0)
		dst[i] = '\0';
	return (src_len);
}

size_t	ft_lens(const char *c)
{
	size_t	i;
	int		in_double_quotes;
	int		in_single_quotes;

	i = 0;
	in_double_quotes = 0;
	in_single_quotes = 0;
	if (operator_check(*c) == 1)
	{
		if ((*c == '<' && *(c + 1) == '<') || (*c == '>' && *(c + 1) == '>'))
			return (2);
		return (1);
	}
	while (*c != '\0' && (operator_check(*c) != 1 || in_double_quotes
			|| in_single_quotes))
	{
		if (*c == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else if (*c == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		c++;
		i++;
	}
	return (i);
}

int	ft_malloc_check(char **arr, size_t i)
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

char	*handle_operator(const char *s)
{
	char	*arr;

	if (s[0] == '<' && s[1] == '<')
	{
		arr = (char *)malloc(sizeof(char) * 3);
		arr[0] = s[0];
		arr[1] = s[1];
		arr[2] = '\0';
	}
	else
	{
		arr = (char *)malloc(sizeof(char) * 2);
		arr[0] = s[0];
		arr[1] = '\0';
	}
	return (arr);
}

char	*handle_word(const char *s)
{
	char	*arr;

	arr = (char *)malloc(sizeof(char) * (ft_lens(s) + 1));
	ft_strlccpy(arr, s, ft_lens(s) + 1);
	return (arr);
}
