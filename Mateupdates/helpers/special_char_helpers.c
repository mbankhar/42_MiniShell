/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_char_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:54:40 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 13:39:59 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	reset_flags(t_special_char_data *data)
{
	if (*data->in_pipe && data->current != ' ')
		*data->in_pipe = 0;
	if (*data->in_redirection && data->current != ' ')
		*data->in_redirection = 0;
	if (*data->out_redirection && data->current != ' ')
		*data->out_redirection = 0;
}

int	handle_special_characters(t_special_char_data *data)
{
	if (data->current == '|')
	{
		if (handle_pipe_error(data->last_char, data->line, data->i))
			return (1);
		*data->in_pipe = 1;
	}
	else if (data->current == '>')
	{
		if (handle_output_redirection(data->line, data->i, data->last_char))
			return (1);
		*data->out_redirection = 1;
	}
	else if (data->current == '<')
	{
		if (handle_input_redirection(data->line, data->i, data->last_char))
			return (1);
		*data->in_redirection = 1;
	}
	else
		reset_flags(data);
	return (0);
}
