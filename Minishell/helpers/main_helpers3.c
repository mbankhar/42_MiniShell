/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 16:23:14 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 16:50:11 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_error_checks(t_check_line_data *data)
{
	if (are_quotes_even(data->line) == 0)
	{
		fprintf(stderr, "Error: Unmatched quotes\n");
		data->shell->exit_status = 2;
		return ;
	}
	if (redirection_error_checks(data->line)
		|| error_check_on_pipe_and_redirection(data->line))
	{
		fprintf(stderr, "Error: Redirection or pipe syntax error\n");
		data->shell->exit_status = 2;
		return ;
	}
}

static void	setup_do_shit_data(t_do_shit_data *do_shit_data, char **args,
		t_check_line_data *data)
{
	do_shit_data->args = args;
	do_shit_data->exec = data->exec;
	do_shit_data->cmds_ptr = &data->cmds;
	do_shit_data->env = *data->environ;
	do_shit_data->shell = data->shell;
}

static void	check_fd_in(t_check_line_data *data, char **args)
{
	int	i;

	i = 0;
	while (i < data->exec->number_of_pipes + 1)
	{
		if (data->cmds[i].fd_in == -1)
		{
			perror(data->cmds[i].cmd_args[0]);
			data->shell->exit_status = 1;
			ft_free(args);
			return ;
		}
		i++;
	}
}

static void	free_heredoc_content1(t_check_line_data *data)
{
	int	i;

	i = 0;
	while (i < data->exec->number_of_pipes + 1)
	{
		free_heredoc_content(&data->cmds[i]);
		i++;
	}
}

void	check_the_line(t_check_line_data *data)
{
	char				**args;
	t_do_shit_data		do_shit_data;

	handle_error_checks(data);
	data->exec->number_of_pipes = count_char_occurrences(data->line, '|');
	args = ft_splitspecial(data->line);
	data->exec->tokens = get_the_token(args);
	setup_do_shit_data(&do_shit_data, args, data);
	do_shit(&do_shit_data);
	check_fd_in(data, args);
	data->cmds->exit_code = 0;
	count_commands(data->cmds, *data->environ);
	if (data->exec->number_of_pipes == data->cmds->cmd_number
		&& (data->exec->number_of_pipes != 0))
		printf("Not nice error\n");
	else
		data->shell->exit_status = execution(data->cmds, data->environ,
				data->shell);
	free_heredoc_content1(data);
	ft_free(args);
}
