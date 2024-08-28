/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:12:26 by mbankhar          #+#    #+#             */
/*   Updated: 2024/08/27 22:21:33 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell(t_shell *shell, char ***environ)
{
	signal(SIGINT, handle_sigint);
	signal(SIGTSTP, handle_sigtstp);
	signal(SIGQUIT, handle_sigquit);
	if ((*environ)[0] == NULL)
		*environ = init_env(*environ);
	shell->exit_status = 0;
}

void	handle_exit_command(char **args, t_handle_line_data *data)
{
	if (handle_exit(args) == 1)
		data->shell->exit_status = 1;
	else
	{
		ft_free(args);
		free(data->line);
		exit(data->shell->exit_status);
	}
}

void	handle_line(t_handle_line_data *data)
{
	char				**args;
	t_check_line_data	check_line_data;

	add_history(data->line);
	args = ft_splitspecial(data->line);
	if (args && args[0] && ft_strcmp(args[0], "exit") == 0)
	{
		handle_exit_command(args, data);
	}
	else
	{
		check_line_data.line = data->line;
		check_line_data.exec = data->exec;
		check_line_data.cmds = data->cmds;
		check_line_data.environ = data->environ;
		check_line_data.shell = data->shell;
		check_the_line(&check_line_data);
	}
	ft_free(args);
}

void	read_loop(t_exec *exec, t_cmds *cmds, char ***environ, t_shell *shell)
{
	char				*line;
	t_handle_line_data	data;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break ;
		if (strlen(line) > 0)
		{
			data.line = line;
			data.exec = exec;
			data.cmds = cmds;
			data.environ = environ;
			data.shell = shell;
			handle_line(&data);
		}
		free(line);
	}
}

int	main(void)
{
	extern char	**environ;
	t_exec		exec;
	t_cmds		*cmds;
	char		**environ_copy;
	t_shell		shell;

	cmds = NULL;
	environ_copy = duplicate_environ(environ);
	init_shell(&shell, &environ_copy);
	read_loop(&exec, cmds, &environ_copy, &shell);
	return (shell.exit_status);
}
