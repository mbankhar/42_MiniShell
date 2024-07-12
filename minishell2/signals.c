/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:48:48 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/12 16:58:19 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_variable;
void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 2);
	if (!g_variable)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void handle_sigtstp(int sig)
{
	(void)sig;
}

void handle_sigquit(int sig)
{
	(void)sig;
}