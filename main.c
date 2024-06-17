/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:12:26 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/17 19:46:43 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

#include <stdlib.h>
#include <string.h>

char	**get_the_token(char **commands, t_exec *exec)
{
	int		i;
	int		cmd_count;
	char	**filtered_commands;
	int		skip_next;
	int		j;
	int		k;

	i = 0;
	cmd_count = 0;
	filtered_commands = NULL;
	skip_next = 0;
	while (commands[i])
	{
		if (skip_next)
			skip_next = 0;
		else if (strcmp(commands[i], "<") == 0 || strcmp(commands[i], ">") == 0)
			skip_next = 1;
		else if (strcmp(commands[i], "|") != 0)
			cmd_count++;
		i++;
	}
	filtered_commands = (char **)malloc((cmd_count + 1) * sizeof(char *));
	if (!filtered_commands)
		return (NULL);
	i = 0;
	j = 0;
	skip_next = 0;
	while (commands[i])
	{
		if (skip_next)
			skip_next = 0;
		else if (strcmp(commands[i], "<") == 0 || strcmp(commands[i], ">") == 0)
			skip_next = 1;
		else if (strcmp(commands[i], "|") != 0)
		{
			filtered_commands[j] = strdup(commands[i]);
			if (!filtered_commands[j])
			{
				k = -1;
				while (++k < j)
				{
					free(filtered_commands[k]);
				}
				free(filtered_commands);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	filtered_commands[j] = NULL;
	return (filtered_commands);
}

char	*get_env_value(char **environ, const char *var)
{
	int		index;
	size_t	len;

	index = 0;
	len = strlen(var);
	while (environ[index])
	{
		// Check if the current environment variable starts with var=
		if (strncmp(environ[index], var, len) == 0 && environ[index][len] == '=')
		{
			return (environ[index] + len + 1); // Return the value part
		}
		index++;
	}
	return (NULL); // Return NULL if the variable is not found
}

// Function to handle $LOGNAME in the commands array
void	handle_dollar(char **commands, int i, int y, char **environ)
{
	char	*var_name;
	char	*env_value;
	// Extract the variable name from the command (e.g., LOGNAME from $LOGNAME)
	var_name = commands[i] + y + 1;

	// Get the value of the environment variable
	env_value = get_env_value(environ, var_name);
	ft_strcpy(commands[i], env_value);
	if (env_value)
	{
		// Replace the command string with the environment variable's value
		commands[i] = env_value;
	}
}

void	check_dollar(char **commands, t_exec *exec)
{
	extern char	**environ;
	int			i;
	int			y;

	i = -1;
	while (commands[++i])
	{
		y = -1;
		while (commands[i][++y])
		{
			if (commands[i][y] == '$')
			{
				// check_for_single_or_double(commands, i);
				handle_dollar(commands, i, y, environ);
			}
		}
	}
}

void	look_for_redirect(char **commands, t_exec *exec)
{
	int	i;
	int	y;
	i = -1;
	while (commands[++i])
	{
		y = -1;
		while (commands[i][++y])
		{
			if (commands[i][y] == '>' && commands[i][y + 1] == '>')
				exec->fd_out = open(commands[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			// else if (commands[i][y] == '<' && commands[i][y + 1] == '<')
			// 	here_doc();
			else if (commands[i][y] == '<')
				exec->fd_in = open(commands[i + 1], O_RDONLY);
			else if (commands[i][y] == '>')
				exec->fd_out = open(commands[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
	}
}

void	check_the_line(char *line, t_exec *exec)
{
	if (are_quotes_even(line) != 0)
	{
		exec->number_of_pipes = count_char_occurrences(line, '|');
		exec->commands = ft_split(line, ' ');
		look_for_redirect(exec->commands, exec);
		check_dollar(exec->commands, exec);
		exec->tokens = get_the_token(exec->commands, exec);
		int	i	= -1;
		while (exec->commands[++i])
			printf("%s\n", exec->commands[i]);
		i	= -1;
		while (exec->tokens[++i])
			printf("%s\n", exec->tokens[i]);
	}
	else
	{
		printf("Quotes error\n");
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	input[MAX_INPUT_SIZE];
	char	*args[MAX_ARG_SIZE];
	t_exec	exec;

	(void)argc;
	(void)argv;
	(void)envp;
	exec.fd_in = -2;
	exec.fd_out = -2;
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break ;
		if (strlen(line) > 0)
		{
			add_history(line);
			check_the_line(line, &exec);
		}
		free(line);
	}
	return (0);
}


// int	main(void)
// {
// 	char	input[MAX_INPUT_SIZE];
// 	char	*args[MAX_ARG_SIZE];
// 	char	*token;
// 	int		should_run = 1;
// 	char		**env;
// 	t_exec	exec;

// 	while (should_run)
// 	{
// 		printf("minishell> ");
// 		fflush(stdout);
// 		if (!fgets(input, MAX_INPUT_SIZE, stdin))
// 			break ;
// 		input[strcspn(input, "\n")] = 0;
// 		int arg_count = 0;
// 		token = strtok(input, " ");
// 		while (token != NULL && arg_count < MAX_ARG_SIZE - 1)
// 		{
// 			args[arg_count++] = token;
// 			token = strtok(NULL, " ");
// 		}
// 		args[arg_count] = NULL;
// 		if (args[0] == NULL)
// 			continue ;
// 		else if (pipes != 0)
// 			execution(pipes);
// 		else if (strcmp(args[0], "cd") == 0)
// 		{
// 			if (args[1] == NULL)
// 				fprintf(stderr, "cd: missing argument\n");
// 			else
// 				change_directory(args[1]);
// 		}
// 		else if (strcmp(args[0], "echo") == 0)
// 			execute_echo(args);
// 		else if (strcmp(args[0], "export") == 0)
// 		{
// 			if (args[1] == NULL)
// 				fprintf(stderr, "export: missing argument\n");
// 			else
// 			{
// 				char *var = strtok(args[1], "=");
// 				char *value = strtok(NULL, "=");
// 				if (value != NULL)
// 					execute_export(var, value);
// 				else if (value == NULL && ft_strchr(args[1], "=" != NULL))
// 					execute_export(var, "");
// 			}
// 		}
// 		else if (strcmp(args[0], "unset") == 0)
// 		{
// 			if (args[1] == NULL)
// 				fprintf(stderr, "unset: missing argument\n");
// 			else
// 				execute_unset(args[1]);
// 		}
// 		else if (strcmp(args[0], "env") == 0)
// 			print_env();
// 		else if (strcmp(args[0], "exit") == 0)
// 			should_run = 0;
// 		else
// 			execute_command(args);
// 	}
// 	return (0);
// }
