/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 18:45:25 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/26 16:15:02 by amohame2         ###   ########.fr       */
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

void execute_export(const char *var, const char *value) {
    // Simulate setting an environment variable by writing to a file
    char filename[256];
    snprintf(filename, sizeof(filename), "%s.env", var);

    // Check if the file can be accessed
    if (access(filename, F_OK) != -1) {
        // File exists, write the value to the file
        FILE *file = fopen(filename, "w");
        if (file != NULL) {
            fprintf(file, "%s", value);
            fclose(file);
            printf("Exported %s to %s with value: %s\n", var, filename, value);
        } else {
            perror("fopen");
        }
    } else {
        // File does not exist, create and write to the file
        FILE *file = fopen(filename, "w");
        if (file != NULL) {
            fprintf(file, "%s", value);
            fclose(file);
            printf("Exported %s to %s with value: %s\n", var, filename, value);
        } else {
            perror("fopen");
        }
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
