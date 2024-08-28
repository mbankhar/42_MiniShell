/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:16:57 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/27 22:22:49 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	operator_check(char str)
{
	if (str == '|' || str == '<' || str == '>' || str == ' ')
		return (1);
	return (0);
}

static int	countwords(const char *str)
{
	t_quote_state	state;

	state.wordcount = 0;
	state.inword = 0;
	state.in_double_quotes = 0;
	state.in_single_quotes = 0;
	while (*str != '\0')
	{
		handle_quotes_state(&str, &state);
		if (operator_check(*str) == 0 && !state.inword
			&& !state.in_double_quotes && !state.in_single_quotes)
		{
			state.inword = 1;
			state.wordcount++;
		}
		str = handle_operator_count(str, &state);
		str++;
	}
	return (state.wordcount);
}

char	**ft_splitspecial(char const *s)
{
	int		i;
	int		counter;
	char	**arr;
	int		p;

	i = 0;
	p = 0;
	counter = countwords(s);
	arr = (char **)malloc(sizeof(char *) * (counter + 1));
	if (arr == NULL)
		return (NULL);
	while (i < counter)
	{
		p = process_operators(s, arr, &i, p);
		if (s[p] == '\0')
			break ;
		arr[i] = handle_word(&s[p]);
		if (!ft_malloc_check(arr, i))
			return (NULL);
		i++;
		p += ft_lens(&s[p]);
	}
	arr[i] = NULL;
	return (arr);
}
