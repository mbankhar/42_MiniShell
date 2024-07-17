/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:12:26 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/17 14:22:45 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>

void	check_the_line(char *line, t_exec *exec, t_cmds *cmds, char ***environ)
{
	char	**args;
	char	delimiter[MAX_DELIMITER_LENGTH];
	char	*delim_start;
	char	*delim_end;
	size_t	delim_len;
	int		heredoc_fd;

	if (are_quotes_even(line) != 0 && !redirection_error_checks(line))
	{
		if (strncmp(line, "<<", 2) == 0)
		{
			delim_start = line + 2;
			while (*delim_start == ' ')
				delim_start++;
			if (*delim_start == '\0')
			{
				printf("Error: No delimiter specified for heredoc\n");
				return ;
			}
			delim_end = delim_start;
			while (*delim_end && !isspace(*delim_end))
				delim_end++;
			delim_len = delim_end - delim_start;
			if (delim_len >= MAX_DELIMITER_LENGTH)
			{
				printf("Error: Delimiter too long\n");
				return ;
			}
			strncpy(delimiter, delim_start, delim_len);
			delimiter[delim_len] = '\0';
			heredoc_fd = handle_heredoc(delimiter);
			if (heredoc_fd != -1)
			{
				execute_command("cat", heredoc_fd);
				close(heredoc_fd);
			}
		}
		else
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
