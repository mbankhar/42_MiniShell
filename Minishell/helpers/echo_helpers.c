/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:23:16 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/27 17:56:36 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_echo_helper(t_cmds *cmds, int *original_stdout,
		int *original_stdin)
{
	*original_stdout = dup(1);
	*original_stdin = dup(0);
	if (cmds->fd_in != -2)
		dup2(cmds->fd_in, 0);
	if (cmds->fd_out != -2)
		dup2(cmds->fd_out, 1);
}

int	handle_echo_flags(t_cmds *cmds, int *i, int *newline)
{
	while (cmds->cmd_args[*i] != NULL && ft_strcmp(cmds->cmd_args[*i],
			"-n") == 0)
	{
		*newline = 0;
		(*i)++;
	}
	return (*newline);
}

void	print_echo_args(t_echo_data *data)
{
	char	*expanded_arg;
	char	*stripped_arg;

	while (data->cmds->cmd_args[*(data->i)] != NULL)
	{
		if (!(*(data->first_arg)))
			printf(" ");
		expanded_arg = expand_variable(data->cmds->cmd_args[*(data->i)],
				data->env, data->shell);
		stripped_arg = strip_quotes(expanded_arg);
		printf("%s", stripped_arg);
		free(expanded_arg);
		free(stripped_arg);
		*(data->first_arg) = 0;
		(*(data->i))++;
	}
}

void	restore_echo_fd(int original_stdout, int original_stdin)
{
	dup2(original_stdout, 1);
	dup2(original_stdin, 0);
	close(original_stdout);
	close(original_stdin);
}
// int	execute_builtin_helper(t_cmds *cmds, char ***env, t_shell *shell)
// {
// 	return (execute_command(cmds, env, shell));
// }