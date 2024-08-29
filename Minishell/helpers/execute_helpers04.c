/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers04.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 14:50:00 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 14:48:32 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execution(t_cmds *cmds, char ***env, t_shell *shell)
{
	return (execution_helper(cmds, env, shell));
}

int	execute_builtin_helper(t_cmds *cmds, char ***env, t_shell *shell)
{
	return (execute_command(cmds, env, shell));
}
