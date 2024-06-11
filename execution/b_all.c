/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_all.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:12:26 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/11 16:14:28 by mbankhar         ###   ########.fr       */
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
	for (int i = 1; args[i] != NULL; i++) {
		printf("%s", args[i]);
		if (args[i + 1] != NULL) {
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
	extern char **environ;
	for (char **env = environ; *env != 0; env++)
	{
		printf("%s\n", *env);
	}
}

void	execute_command(char *args[])
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
	}
}

void	env_function(char *first, char **args)
{
	char		**splitted;
	static char	**storage;
	char		**new_storage;
	static int	i;
	int			y;

	// if (strcmp(args[0], "export") == 0)
	// {
	// 	if (args[1] == NULL)
	// 		fprintf(stderr, "export: missing argument\n");
	// 	else
	// 	{
	// 		char *var = strtok(args[1], "=");
	// 		char *value = strtok(NULL, "=");
	// 		if (value != NULL)
	// 			execute_export(var, value);
	// 		else if (value == "")
	// 			execute_export(var, "");
	// 		else
	// 			fprintf(stderr, "export: invalid format, use VAR=VALUE\n");
	// 	}
	// }
	if (storage == NULL)
	{
		i = 1;
		storage = malloc(sizeof(char *) * i + 1);
		storage[0] = malloc(sizeof(char) * ft_strlen(first) + 1);
		storage[1] = NULL;
		ft_strcpy(storage[0], first);
	}
	else
	{
		i++;
		new_storage = malloc(sizeof(char *) * i + 1);
		new_storage[i] = NULL;
		y = 0;
		while (y < i - 1)
		{
			new_storage[y] = malloc(sizeof(char) * ft_strlen(storage[y]));
			y++;
		}
		y = 0;
		while (storage[y] != NULL)
		{
			ft_strcpy(new_storage[y], storage[y]);
			y++;
		}
		new_storage[i - 1] = malloc(sizeof(char) * ft_strlen(first) +1);
		ft_strcpy(new_storage[y], first);
		ft_free(storage);
		storage = new_storage;
		i = -1;
		while (storage[++i] != NULL)
			printf("%s\n", storage[i]);
	}
}

int	main()
{
	char	input[MAX_INPUT_SIZE];
	char	*args[MAX_ARG_SIZE];
	char	*token;
	int		should_run = 1;

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
		else if (strcmp(args[0], "cd") == 0)
		{
			if (args[1] == NULL)
				fprintf(stderr, "cd: missing argument\n");
			else
				change_directory(args[1]);
		}
		// else if ((strnstr(args[0], "=", strlen(args[0])) != NULL && strcmp(args[0],strnstr(args[0], "=", strlen(args[0]))) != 0) || strcmp(args[0], "export") == 0)
		// 	env_function(args[0], args);
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
				printf("%s", args[1]);
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
