/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers01.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 22:49:54 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 14:05:22 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_first_command(t_cmds *cmds, int i, int pipefd[2])
{
	if (cmds[i].fd_in != -2)
		dup2(cmds[i].fd_in, STDIN_FILENO);
	if (cmds[i].fd_out != -2)
		dup2(cmds[i].fd_out, STDOUT_FILENO);
	else
		dup2(pipefd[1], STDOUT_FILENO);
}

void	setup_last_command(t_cmds *cmds, int i, int prev_fd)
{
	if (cmds[i].fd_in != -2)
		dup2(cmds[i].fd_in, STDIN_FILENO);
	else
		dup2(prev_fd, STDIN_FILENO);
	if (cmds[i].fd_out != -2)
		dup2(cmds[i].fd_out, STDOUT_FILENO);
}

void	setup_middle_command(t_cmds *cmds, int i, int pipefd[2], int prev_fd)
{
	if (cmds[i].fd_in != -2)
		dup2(cmds[i].fd_in, STDIN_FILENO);
	else
		dup2(prev_fd, STDIN_FILENO);
	if (cmds[i].fd_out != -2)
		dup2(cmds[i].fd_out, STDOUT_FILENO);
	else
		dup2(pipefd[1], STDOUT_FILENO);
}

void	setup_pipes_and_fds(t_cmds *cmds, int i, int pipefd[2], int prev_fd)
{
	if (i == 0)
		setup_first_command(cmds, i, pipefd);
	else if (i == cmds->size - 1)
		setup_last_command(cmds, i, prev_fd);
	else
		setup_middle_command(cmds, i, pipefd, prev_fd);
}

void	handle_child_process(t_cmds *cmds, char ***env, t_shell *shell, int i)
{
	int	pipefd[2];
	int	prev_fd;
	int	heredoc_pipe[2];

	prev_fd = -1;
	if (i < cmds->size - 1 && pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	setup_pipes_and_fds(cmds, i, pipefd, prev_fd);
	handle_heredoc_pipe(cmds, i, heredoc_pipe);
	execute(*env, cmds[i].cmd_args, shell);
	exit(EXIT_SUCCESS);
}
