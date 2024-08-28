/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:48:48 by mbankhar          #+#    #+#             */
/*   Updated: 2024/08/27 22:17:04 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	int	g_variable;

	g_variable = 0;
	(void)sig;
	write(1, "\n", 2);
	if (!g_variable)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_sigtstp(int sig)
{
	(void)sig;
}

void	handle_sigquit(int sig)
{
	(void)sig;
}
