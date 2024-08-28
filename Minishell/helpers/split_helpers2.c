/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_helpers2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:15:04 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 13:39:47 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_quotes(const char *s, int *p)
{
	char	quote;

	if (s[*p] == '"' || s[*p] == '\'')
	{
		quote = s[*p];
		(*p)++;
		while (s[*p] != quote && s[*p])
			(*p)++;
	}
}

int	process_operators(const char *s, char **arr, int *i, int p)
{
	while (operator_check(s[p]) == 1)
	{
		if (operator_check(s[p]) == 1 && s[p] != ' ')
		{
			arr[*i] = handle_operator(&s[p]);
			if (s[p] == '<' && s[p + 1] == '<')
				p++;
			(*i)++;
		}
		p++;
	}
	return (p);
}

void	handle_quotes_state(const char **str, t_quote_state *state)
{
	if (**str == '"' && !(state->in_single_quotes))
		state->in_double_quotes = !(state->in_double_quotes);
	else if (**str == '\'' && !(state->in_double_quotes))
		state->in_single_quotes = !(state->in_single_quotes);
}

const char	*handle_operator_count(const char *str, t_quote_state *state)
{
	if ((operator_check(*str) == 1) && !isspace(*str))
	{
		if ((*str == '<' && *(str + 1) == '<') || (*str == '>' && *(str
					+ 1) == '>'))
			str++;
		(state->wordcount)++;
		state->inword = 0;
	}
	if (operator_check(*str) == 1 && !state->in_double_quotes
		&& !state->in_single_quotes)
		state->inword = 0;
	return (str);
}
