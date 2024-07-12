/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:12:26 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/12 16:57:50 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_the_line(char *line, t_exec *exec, t_cmds *cmds, char ***environ)
{
	char	**args;

	// int          *exit;
	// if (ft_strcmp(line, "exit"))
	//  exit = handle_exit();
	if (are_quotes_even(line) != 0 && !redirection_error_checks(line))
	{
		exec->number_of_pipes = count_char_occurrences(line, '|');
		args = ft_splitspecial(line);
		exec->tokens = get_the_token(args);
		do_shit(args, exec, &cmds, *environ);
		cmds->exit_code = 0;
		count_commands(cmds, *environ);
		if (exec->number_of_pipes == cmds->cmd_number
			&& (exec->number_of_pipes != 0))
			printf("Not nice error\n");
		else
			execution(cmds, environ);
		ft_free(args);
	}
	else
		printf(" syntax error near unexpected token\n");
}
int	*get_exit(void)
{
	static int	exitcode = 0;

	return (&exitcode);
}
char	*take_line(char *msg)
{
	char	*line;

	if (isatty(fileno(stdin)))
		return (readline(msg));
	else
	{
		line = get_next_line(fileno(stdin));
		if (line && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		return (line);
	}
}
int	main(void)
{
	extern char	**environ;
	char		*line;
	t_exec		exec;
	t_cmds		*cmds;
	char		**environ_copy;

	cmds = NULL;
	if (environ[0] == NULL)
		environ = init_env(environ);
	else
		environ_copy = duplicate_environ(environ);
	while (1)
	{
		// g_variable = 0;
		line = take_line("minishell> ");
		if (!line)
			break ;
		if (strlen(line) > 0)
		{
			add_history(line);
			if (ft_strcmp(line, "exit") == 0)
			{
				free(line);
				if (isatty(fileno(stdin)))
					printf("exit\n");
				break ;
			}
			// handle_exit(line);
			check_the_line(line, &exec, cmds, &environ_copy);
		}
		free(line);
	}
	return (0);
}