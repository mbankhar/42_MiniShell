/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 15:45:44 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/26 11:45:38 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	operator_check(char str)
{
	if (str == '|' || str == '\'' || str == '<' || str == '>' || str == ' ')
	{
		return (1);
	}
	return (0);
}

static size_t	ft_strlccpy(char *dst, const char *src, size_t dstsize)
{
	size_t	y;
	size_t	i;
	size_t	src_len;

	y = 0;
	i = 0;
	while (src[y] != '\0')
	{
		y++;
	}
	src_len = y;
	if (dstsize == 0)
	{
		return (src_len);
	}
	while (i < dstsize - 1 && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	if (dstsize != 0)
	{
		dst[i] = '\0';
	}
	return (src_len);
}

static	size_t	ft_lens(const char *c, char b)
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
		{
			return (2);
		}
		return (1);
	}
	while (*c != '\0' && (operator_check(*c) != 1|| in_double_quotes || in_single_quotes))
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

static int	countwords(const char *str, char c)
{
	int	wordcount;
	int	inword;
	int	in_double_quotes;
	int	in_single_quotes;

	wordcount = 0;
	inword = 0;
	in_double_quotes = 0;
	in_single_quotes = 0;
	while (*str != '\0')
	{
		if (*str == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
		}
		else if (*str == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
		}
		if (operator_check(*str) == 0 && !inword && !in_double_quotes && !in_single_quotes)
		{
			inword = 1;
			wordcount++;
		}
		if ((operator_check(*str) == 1) && !isspace(*str))
		{
			if ((*str == '<' && *(str + 1) == '<') || (*str == '>' && *(str + 1) == '>'))
			{
				str++;  // Skip next character for << and >>
			}
			wordcount++;
			inword = 0;
		}
		if (operator_check(*str) == 1 && !in_double_quotes && !in_single_quotes)
		{
			inword = 0;
		}
		str++;
	}
	return (wordcount);
}

static int	ft_malloc_check(char **arr, size_t i)
{
	int	temp;

	if (arr[i] == NULL)
	{
		temp = i;
		while (--temp >= 0)
		{
			free(arr[temp]);
		}
		free(arr);
		return (0);
	}
	return (1);
}

char	**ft_splitspecial(char const *s, char c)
{
	int		i;
	int		counter;
	char	**arr;
	int		p;
	char	quote;

	p = 0;
	counter = countwords(s, c);
	i = 0;
	arr = (char **)malloc(sizeof(char *) * (counter + 1));
	if (arr == NULL)
	{
		return (NULL);
	}
	while (i < counter)
	{
		while (operator_check(s[p]) == 1)
		{
			if (operator_check(s[p]) == 1 && s[p] != ' ')
			{
				if ((s[p] == '<' && s[p + 1] == '<') || (s[p] == '>' && s[p + 1] == '>'))
				{
					arr[i] = (char *)malloc(sizeof(char) * 3);
					arr[i][0] = s[p];
					arr[i][1] = s[p + 1];
					arr[i][2] = '\0';
					p++;
				}
				else
				{
					arr[i] = (char *)malloc(sizeof(char) * 2);
					arr[i][0] = s[p];
					arr[i][1] = '\0';
				}
				i++;
			}
			p++;
		}
		arr[i] = (char *)malloc(sizeof(char) * (ft_lens(&s[p], c) + 1));
		if (!ft_malloc_check(arr, i))
		{
			return (NULL);
		}
		ft_strlccpy(arr[i], &s[p], ft_lens(&s[p], c) + 1);
		i++;
		while (operator_check(s[p]) == 0 && s[p])
		{
			if (s[p] == '"' || s[p] == '\'')
			{
				quote = s[p];
				p++;
				while (s[p] != quote && s[p])
				{
					p++;
				}
			}
			p++;
		}
	}
	arr[i] = NULL;
	return (arr);
}
