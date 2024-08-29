/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:00:00 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/27 17:53:39 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_pipe_error(char last, char *line, int *i)
{
	int	j;

	if (strncmp(line, "echo", 4) == 0 && isspace(line[4]))
	{
		j = 4;
		while (isspace(line[j]))
			j++;
		if (*i >= j)
			return (0);
	}
	if (last == '|' || last == '\0')
	{
		printf("Error: Invalid pipe usage.\n");
		return (1);
	}
	return (0);
}

int	check_echo_whitespace(char *line, int *i)
{
	int	j;

	j = 4;
	while (isspace(line[j]))
		j++;
	return (*i >= j);
}

int	handle_output_redirection(char *line, int *i, char last)
{
	if (strncmp(line, "echo", 4) == 0 && isspace(line[4])
		&& check_echo_whitespace(line, i))
		return (0);
	if (line[*i + 1] == '>')
	{
		if (last == '>' || last == '\0')
		{
			printf("Error: Invalid append redirection.\n");
			return (1);
		}
		(*i)++;
	}
	else if (last == '>' || last == '\0')
	{
		printf("Error: Invalid output redirection.\n");
		return (1);
	}
	return (0);
}

int	handle_input_redirection(char *line, int *i, char last)
{
	if (line[*i + 1] == '<')
	{
		(*i)++;
		return (0);
	}
	if (last == '<' || last == '\0')
	{
		printf("Error: Invalid input redirection.\n");
		return (1);
	}
	return (0);
}

int	error_check_end_conditions(char last_char, int in_pipe, int in_redirection,
		int out_redirection)
{
	if (last_char == '|' || last_char == '>' || last_char == '<')
	{
		printf("Error: Incomplete command.\n");
		return (1);
	}
	if (in_pipe || in_redirection || out_redirection)
	{
		printf("Error: Improper command ending.\n");
		return (1);
	}
	return (0);
}
