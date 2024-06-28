/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 18:45:25 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/28 11:14:34 by mbankhar         ###   ########.fr       */
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
	if (access(var, F_OK) != -1)
	{
		if (unlink(var) == -1)
		{
			perror("unlink");
		}
	}
	else
	{
		perror("access");
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
