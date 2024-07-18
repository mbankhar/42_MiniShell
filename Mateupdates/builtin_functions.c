/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 18:45:25 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/18 18:20:08 by amohame2         ###   ########.fr       */
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

// Function to strip quotes from a string
char	*strip_quotes(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\''
				&& str[len - 1] == '\'')))
	{
		str[len - 1] = '\0';
		str++;
	}
	return (str);
}

// Function to convert an integer to a string
char	*itoa(int num)
{
	int		tmp;
	int		len;
	char	*str;

	tmp = num;
	len = 0;
	if (num == 0)
		return (ft_strdup("0"));
	while (tmp != 0)
	{
		len++;
		tmp /= 10;
	}
	str = malloc(len + 1);
	if (!str)
	{
		exit(EXIT_FAILURE);
	}
	str[len] = '\0';
	while (num != 0)
	{
		str[--len] = '0' + (num % 10);
		num /= 10;
	}
	return (str);
}

char	*expand_variable(char *str, char **env, t_cmds *cmds)
{
	char	*var_name;
	int		i;

	if (str[0] == '$')
	{
		if (str[1] == '?')
		{
			return (itoa(cmds->exit_code));
		}
		else
		{
			var_name = str + 1;
			i = -1;
			while (env[++i] != NULL)
			{
				if (ft_strncmp(env[i], var_name, ft_strlen(var_name)) == 0
					&& env[i][ft_strlen(var_name)] == '=')
				{
					return (env[i] + ft_strlen(var_name) + 1);
				}
			}
		}
	}
	return (str);
}

// Function to execute the echo command
// void	execute_echo(t_cmds *cmds, char **env)
// {
// 	int		i;
// 	int		newline;
// 	int		original_stdout;
// 	int		original_stdin;
// 	char	*expanded_arg;

// 	i = 0;
// 	newline = 1;
// 	original_stdout = dup(1);
// 	original_stdin = dup(0);
// 	if (cmds->fd_in != -2)
// 		dup2(cmds->fd_in, 0);
// 	if (cmds->fd_out != -2)
// 		dup2(cmds->fd_out, 1);
// 	if (cmds->cmd_args[++i] != NULL && ft_strcmp(cmds->cmd_args[i], "-n") == 0)
// 		newline = 0;
// 	while (cmds->cmd_args[i] != NULL)
// 	{
// 		expanded_arg = expand_variable(cmds->cmd_args[i], env, cmds);
// 		printf("%s", strip_quotes(expanded_arg));
// 		if (cmds->cmd_args[i + 1] != NULL)
// 			printf(" ");
// 		i++;
// 	}
// 	if (newline)
// 		printf("\n");
// 	dup2(original_stdout, 1);
// 	dup2(original_stdin, 0);
// 	close(original_stdout);
// 	close(original_stdin);
// }

void	execute_echo(t_cmds *cmds, char **env)
{
	int		i;
	int		newline;
	int		original_stdout;
	int		original_stdin;
	char	*expanded_arg;
	int		first_arg;

	i = 1; // Start from the first argument after "echo"
	newline = 1;
	original_stdout = dup(1);
	original_stdin = dup(0);
	if (cmds->fd_in != -2)
		dup2(cmds->fd_in, 0);
	if (cmds->fd_out != -2)
		dup2(cmds->fd_out, 1);
	// Handle multiple -n options
	while (cmds->cmd_args[i] != NULL && ft_strcmp(cmds->cmd_args[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	// Print arguments
	first_arg = 1;
	while (cmds->cmd_args[i] != NULL)
	{
		if (!first_arg)
			printf(" ");
		expanded_arg = expand_variable(cmds->cmd_args[i], env, cmds);
		printf("%s", strip_quotes(expanded_arg));
		first_arg = 0;
		i++;
	}
	if (newline)
		printf("\n");
	dup2(original_stdout, 1);
	dup2(original_stdin, 0);
	close(original_stdout);
	close(original_stdin);
}

void	print_env(char **env)
{
	int		i;
	char	*equal_sign;

	i = 0;
	while (env[i] != NULL)
	{
		equal_sign = ft_strchr(env[i], '=');
		if (equal_sign != NULL)
		{
			printf("%s\n", env[i]);
		}
		i++;
	}
}

void	pwd(char **env)
{
	char	*pwd_value;
	int		i;

	pwd_value = NULL;
	i = -1;
	while (env[++i] != NULL)
	{
		if (strncmp(env[i], "PWD=", 4) == 0)
		{
			pwd_value = env[i] + 4;
			break ;
		}
	}
	if (pwd_value != NULL)
		printf("%s\n", pwd_value);
	else
		fprintf(stderr, "PWD not found in the environment.\n");
}
