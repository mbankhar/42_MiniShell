/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helpers1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 18:02:44 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 14:53:04 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env_helper(char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strchr(env[i], '=') != NULL)
		{
			printf("%s\n", env[i]);
		}
		i++;
	}
}

static int	is_valid_option(char *option)
{
	return (strcmp(option, "-L") == 0 || strcmp(option, "-P") == 0);
}

static int	handle_invalid_option(char *option)
{
	fprintf(stderr, "pwd: invalid option -- '%c'\n", option[1]);
	return (1);
}

static int	print_current_directory(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("getcwd");
		return (1);
	}
}

int	execute_pwd_helper(char **cmd_args)
{
	int	i;

	i = 1;
	while (cmd_args[i] != NULL)
	{
		if (cmd_args[i][0] == '-' && !is_valid_option(cmd_args[i]))
			return (handle_invalid_option(cmd_args[i]));
		i++;
	}
	return (print_current_directory());
}
