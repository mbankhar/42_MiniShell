/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 11:57:49 by amohame2          #+#    #+#             */
/*   Updated: 2024/07/02 12:00:47 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

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

void execute_export(const char *var, const char *value)
{
	extern char **environ;
	char *new_entry = malloc(strlen(var) + strlen(value) + 2);
	if (!new_entry) {
		perror("malloc");
		return;
	}

	sprintf(new_entry, "%s=%s", var, value);

	int i = 0;
	while (environ[i])
	{
		i++;
	}
	// Reallocate the environ array to add the new entry
	char **new_environ = realloc(environ, (i + 2) * sizeof(char *));
	if (!new_environ) {
		perror("realloc");
		free(new_entry); // Clean up
		return;
	}
	environ = new_environ;

	environ[i] = new_entry;
	environ[i + 1] = NULL;
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

void	execute_unset(char *var)
{
	extern char **environ;
	int i = 0, j = 0;
	while (environ[i])
	{
		if (strncmp(environ[i], var, strlen(var)) != 0 || environ[i][strlen(var)] != '=')
		{
			environ[j++] = environ[i];
		}
		i++;
	}
	environ[j] = NULL;
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
