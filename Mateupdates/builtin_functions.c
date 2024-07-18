/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 18:45:25 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/18 22:04:27 by amohame2         ###   ########.fr       */
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

char *strip_quotes(char *str)
{
    int len = ft_strlen(str);
    char *result = malloc(len + 1);
    int i, j;

    for (i = 0, j = 0; i < len; i++)
    {
        if (str[i] == '\'' || str[i] == '\"')
        {
            char quote = str[i];
            i++;
            while (i < len && str[i] != quote)
            {
                result[j++] = str[i++];
            }
        }
        else
        {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';

    return result;
}

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

char *expand_variable(char *str, char **env, t_cmds *cmds)
{
    char *result = ft_strdup("");
    char *temp;
    int i = 0;

    while (str[i])
    {
        if (str[i] == '\'' && (i == 0 || str[i-1] != '\\'))
        {
            int j = i + 1;
            while (str[j] && str[j] != '\'')
                j++;
            temp = ft_strndup(str + i, j - i + 1);
            char *new_result = ft_strjoin(result, temp);
            free(result);
            free(temp);
            result = new_result;
            i = j + 1;
        }
        else if (str[i] == '$' && str[i+1] && (str[i+1] == '?' || ft_isalnum(str[i+1]) || str[i+1] == '_'))
        {
            i++;
            if (str[i] == '?')
            {
                temp = ft_itoa(cmds->exit_code);
                char *new_result = ft_strjoin(result, temp);
                free(result);
                free(temp);
                result = new_result;
                i++;
            }
            else
            {
                int j = i;
                while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
                    j++;
                char *var_name = ft_strndup(str + i, j - i);
                char *var_value = get_env_values(env, var_name);
                if (var_value)
                {
                    char *new_result = ft_strjoin(result, var_value);
                    free(result);
                    result = new_result;
                }
                free(var_name);
                i = j;
            }
        }
        else
        {
            temp = ft_strndup(str + i, 1);
            char *new_result = ft_strjoin(result, temp);
            free(result);
            free(temp);
            result = new_result;
            i++;
        }
    }

    return result;
}

void execute_echo(t_cmds *cmds, char **env)
{
    int     i;
    int     newline;
    int     original_stdout;
    int     original_stdin;
    char    *expanded_arg;
    int     first_arg;

    i = 1;
    newline = 1;
    original_stdout = dup(1);
    original_stdin = dup(0);
    
    if (cmds->fd_in != -2)
        dup2(cmds->fd_in, 0);
    if (cmds->fd_out != -2)
        dup2(cmds->fd_out, 1);

    while (cmds->cmd_args[i] != NULL && ft_strcmp(cmds->cmd_args[i], "-n") == 0)
    {
        newline = 0;
        i++;
    }

    first_arg = 1;
    while (cmds->cmd_args[i] != NULL)
    {
        if (!first_arg)
            printf(" ");

        expanded_arg = expand_variable(cmds->cmd_args[i], env, cmds);
        char *stripped_arg = strip_quotes(expanded_arg);
        printf("%s", stripped_arg);
        
        free(expanded_arg);
        free(stripped_arg);
        
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