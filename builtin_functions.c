/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 18:45:25 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/26 14:41:11 by mbankhar         ###   ########.fr       */
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
