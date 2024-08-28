/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helpers2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:01:46 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/27 18:01:51 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env(char **env)
{
	print_env_helper(env);
}

int	execute_pwd(char **cmd_args)
{
	return (execute_pwd_helper(cmd_args));
}
