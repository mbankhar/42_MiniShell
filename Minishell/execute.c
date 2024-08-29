/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:18:10 by mbankhar          #+#    #+#             */
/*   Updated: 2024/08/28 14:41:15 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

int	is_builtin(t_cmds *cmds)
{
	return (is_builtin_helper(cmds));
}

int	handle_redirections(char **cmd, t_cmds *cmds, char **env, t_shell *shell)
{
	return (handle_redirections_helper(cmd, cmds, env, shell));
}

int	execute_builtin(t_cmds *cmds, char ***env, t_shell *shell)
{
	return (execute_builtin_helper(cmds, env, shell));
}

int	*get_exit(void)
{
	return (get_exit_helper());
}

void	execute(char **env, char **cmd, t_shell *shell)
{
	execute_helper(env, cmd, shell);
}
