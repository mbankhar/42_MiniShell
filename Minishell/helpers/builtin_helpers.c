/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 12:57:23 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/27 17:43:16 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	copy_without_quotes(char *str, char *result, int *i, int *j)
{
	char	quote;

	quote = str[(*i)++];
	while (str[*i] && str[*i] != quote)
	{
		result[(*j)++] = str[(*i)++];
	}
}

char	*strip_quotes_helper(char *str)
{
	int		len;
	char	*result;
	int		i;
	int		j;

	len = ft_strlen(str);
	result = malloc(len + 1);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			copy_without_quotes(str, result, &i, &j);
		}
		else
		{
			result[j++] = str[i++];
		}
	}
	result[j] = '\0';
	return (result);
}

char	*itoa_helper(int num)
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
		exit(EXIT_FAILURE);
	str[len] = '\0';
	while (num != 0)
	{
		str[--len] = '0' + (num % 10);
		num /= 10;
	}
	return (str);
}

char	*expand_variable_helper(char *str, char **env, t_shell *shell)
{
	char	*result;
	char	*temp;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && (i == 0 || str[i - 1] != '\\'))
			temp = handle_single_quotes(str, &i);
		else if (str[i] == '$' && str[i + 1] && (str[i + 1] == '?'
				|| ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
			temp = handle_dollar_sign(str, env, shell, &i);
		else
			temp = handle_regular_char(str, &i);
		result = join_and_free(result, temp);
	}
	return (result);
}

int	execute_echo_helper(t_cmds *cmds, char **env, t_shell *shell)
{
	t_echo_helper_vars		vars;

	vars.i = 1;
	vars.newline = 1;
	vars.first_arg = 1;
	setup_echo_helper(cmds, &vars.original_stdout, &vars.original_stdin);
	vars.newline = handle_echo_flags(cmds, &vars.i, &vars.newline);
	vars.data.cmds = cmds;
	vars.data.env = env;
	vars.data.shell = shell;
	vars.data.i = &vars.i;
	vars.data.first_arg = &vars.first_arg;
	print_echo_args(&vars.data);
	if (vars.newline)
		printf("\n");
	restore_echo_fd(vars.original_stdout, vars.original_stdin);
	return (0);
}
