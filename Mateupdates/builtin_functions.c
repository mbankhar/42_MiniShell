/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 18:45:25 by mbankhar          #+#    #+#             */
/*   Updated: 2024/08/27 22:43:58 by amohame2         ###   ########.fr       */
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

char	*strip_quotes(char *str)
{
	return (strip_quotes_helper(str));
}

char	*itoa(int num)
{
	return (itoa_helper(num));
}

char	*expand_variable(char *str, char **env, t_shell *shell)
{
	return (expand_variable_helper(str, env, shell));
}

int	execute_echo(t_cmds *cmds, char **env, t_shell *shell)
{
	return (execute_echo_helper(cmds, env, shell));
}
