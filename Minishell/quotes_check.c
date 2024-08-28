/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 15:52:31 by mbankhar          #+#    #+#             */
/*   Updated: 2024/08/27 22:33:34 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_char_occurrences(const char *str, char ch)
{
	int	count;
	int	in_single_quote;
	int	in_double_quote;

	count = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (*str)
	{
		if (*str == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
		}
		else if (*str == '\"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
		}
		else if (*str == ch && !in_single_quote && !in_double_quote)
		{
			count++;
		}
		str++;
	}
	return (count);
}

int	are_quotes_even(const char *str)
{
	int	double_quote_count;
	int	single_quote_count;
	int	in_double_quotes;
	int	in_single_quotes;

	double_quote_count = 0;
	single_quote_count = 0;
	in_double_quotes = 0;
	in_single_quotes = 0;
	while (*str)
	{
		if (*str == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			double_quote_count++;
		}
		else if (*str == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			single_quote_count++;
		}
		str++;
	}
	return ((double_quote_count % 2 == 0) && (single_quote_count % 2 == 0));
}
