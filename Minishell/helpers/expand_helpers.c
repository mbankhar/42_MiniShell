/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:19:51 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 13:41:00 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*handle_single_quotes(char *str, int *i)
{
	int		j;
	char	*temp;

	j = *i + 1;
	while (str[j] && str[j] != '\'')
		j++;
	temp = ft_strndup(str + *i, j - *i + 1);
	*i = j + 1;
	return (temp);
}

char	*handle_dollar_sign(char *str, char **env, t_shell *shell, int *i)
{
	int		j;
	char	*var_name;
	char	*var_value;
	char	*temp;

	(*i)++;
	if (str[*i] == '?')
	{
		temp = ft_itoa(shell->exit_status);
		(*i)++;
		return (temp);
	}
	j = *i;
	while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
		j++;
	var_name = ft_strndup(str + *i, j - *i);
	var_value = get_env_values(env, var_name);
	free(var_name);
	*i = j;
	if (var_value)
		return (ft_strdup(var_value));
	return (ft_strdup(""));
}

char	*handle_regular_char(char *str, int *i)
{
	char	*temp;

	temp = ft_strndup(str + *i, 1);
	(*i)++;
	return (temp);
}

char	*join_and_free(char *s1, char *s2)
{
	char	*new_str;

	new_str = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (new_str);
}
