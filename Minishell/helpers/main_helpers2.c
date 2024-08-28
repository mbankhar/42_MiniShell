/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:00:00 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 13:40:44 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_special_char(t_check_syntax_data *data, char last_char,
		int i)
{
	t_special_char_data	special_char_data;

	special_char_data.current = data->line[i];
	special_char_data.last_char = last_char;
	special_char_data.in_pipe = data->in_pipe;
	special_char_data.in_redirection = data->in_redirection;
	special_char_data.out_redirection = data->out_redirection;
	special_char_data.i = &i;
	special_char_data.line = data->line;
	if (handle_special_characters(&special_char_data))
		return (1);
	return (0);
}

int	check_syntax(t_check_syntax_data *data)
{
	char	last_char;
	int		i;

	last_char = '\0';
	i = 0;
	while (i < data->len)
	{
		if (isspace(data->line[i]))
		{
			i++;
			continue ;
		}
		if (handle_special_char(data, last_char, i))
			return (1);
		last_char = data->line[i];
		i++;
	}
	return (error_check_end_conditions(last_char, *data->in_pipe,
			*data->in_redirection, *data->out_redirection));
}

int	error_check_on_pipe_and_redirection(char *line)
{
	int					len;
	int					in_redirection;
	int					out_redirection;
	int					in_pipe;
	t_check_syntax_data	data;

	len = strlen(line);
	in_redirection = 0;
	out_redirection = 0;
	in_pipe = 0;
	data.line = line;
	data.len = len;
	data.in_pipe = &in_pipe;
	data.in_redirection = &in_redirection;
	data.out_redirection = &out_redirection;
	return (check_syntax(&data));
}

int	is_special_char(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == ';' || c == '&');
}

int	is_quoted(char *str, int index)
{
	int	in_single_quote;
	int	in_double_quote;
	int	i;

	in_single_quote = 0;
	in_double_quote = 0;
	i = 0;
	while (i < index)
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	return (in_single_quote || in_double_quote);
}
