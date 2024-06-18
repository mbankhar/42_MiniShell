/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:12:26 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/18 12:51:38 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100



void	check_the_line(char *line, t_exec *exec)
{
	int			i;
	extern char	**environ;

	if (are_quotes_even(line) != 0)
	{
		exec->number_of_pipes = count_char_occurrences(line, '|');
		exec->commands = ft_split(line, ' ');
		look_for_redirect(exec->commands, exec);
		check_dollar(exec->commands, exec);
		exec->tokens = get_the_token(exec->commands, exec);
		exec->num_commands = 0;
		exec->num_commands = get_token_number(exec->tokens, environ);
		// i = -1;
		// while (exec->commands[++i])
		// 	printf("%s\n", exec->commands[i]);
		// i = -1;
		// while (exec->tokens[++i])
		// 	printf("%s\n", exec->tokens[i]);
		// exit(1);
		execution(exec->tokens, environ, exec);
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
