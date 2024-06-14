/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_all.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:12:26 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/13 14:21:37 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

void	ft_free(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	change_directory(char *path)
{
	if (chdir(path) == -1)
	{
		perror("chdir");
	}
}

void	execute_echo(char *args[])
{
	int	i;

	i = 0;
	while (args[++i] != NULL)
	{
		printf("%s", args[i]);
		if (args[i + 1] != NULL)
		{
			printf(" ");
		}
	}
	printf("\n");
}

void	execute_export(char *var, char *value)
{
	if (setenv(var, value, 1) == -1)
	{
		perror("setenv");
	}
}

void	execute_unset(char *var)
{
	if (unsetenv(var) == -1)
	{
		perror("unsetenv");
	}
}

void	print_env()
{
	extern char	**environ;
	char		**env;

	env = environ;
	while (*env != 0)
	{
		printf("%s\n", *env);
		env++;
	}
}

void	execute_command(char *args[])
{
	pid_t			pid;
	char			*path;
	char			**env;
	int				status;
	extern char		**environ;

	env = environ;
	path = get_path(env, args[0]);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		if (execve(path, &args[0], args) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
		waitpid(pid, &status, 0);
}

int	main(void)
{
	char	input[MAX_INPUT_SIZE];
	char	*args[MAX_ARG_SIZE];
	char	*token;
	int		should_run = 1;
	int		pipes;
	char	*commands[] = {"ls","grep test", "wc -l"};
	int		num_commands = sizeof(commands) / sizeof(commands[0]);
	char		**env;

	while (should_run)
	{
		printf("minishell> ");
		fflush(stdout);
		if (!fgets(input, MAX_INPUT_SIZE, stdin))
			break ;
		input[strcspn(input, "\n")] = 0;
		int arg_count = 0;
		token = strtok(input, " ");
		while (token != NULL && arg_count < MAX_ARG_SIZE - 1)
		{
			args[arg_count++] = token;
			token = strtok(NULL, " ");
		}
		args[arg_count] = NULL;
		if (args[0] == NULL)
			continue ;
		else if (pipes != 0)
			execution(pipes);
		else if (strcmp(args[0], "cd") == 0)
		{
			if (args[1] == NULL)
				fprintf(stderr, "cd: missing argument\n");
			else
				change_directory(args[1]);
		}
		else if (strcmp(args[0], "echo") == 0)
			execute_echo(args);
		else if (strcmp(args[0], "export") == 0)
		{
			if (args[1] == NULL)
				fprintf(stderr, "export: missing argument\n");
			else
			{
				char *var = strtok(args[1], "=");
				char *value = strtok(NULL, "=");
				if (value != NULL)
					execute_export(var, value);
				else if (value == NULL && ft_strchr(args[1], "=" != NULL))
					execute_export(var, "");
			}
		}
		else if (strcmp(args[0], "unset") == 0)
		{
			if (args[1] == NULL)
				fprintf(stderr, "unset: missing argument\n");
			else
				execute_unset(args[1]);
		}
		else if (strcmp(args[0], "env") == 0)
			print_env();
		else if (strcmp(args[0], "exit") == 0)
			should_run = 0;
		else
			execute_command(args);
	}
	return (0);
}
