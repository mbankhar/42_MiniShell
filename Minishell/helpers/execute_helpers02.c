/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers02.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 22:51:08 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 14:03:48 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_heredoc_pipe(t_cmds *cmds, int i, int heredoc_pipe[2])
{
	int	j;

	if (cmds[i].heredoc_content)
	{
		if (pipe(heredoc_pipe) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		j = 0;
		while (j < cmds[i].heredoc_line_count)
		{
			if (cmds[i].heredoc_content[j])
			{
				write(heredoc_pipe[1], cmds[i].heredoc_content[j],
					strlen(cmds[i].heredoc_content[j]));
				write(heredoc_pipe[1], "\n", 1);
			}
			j++;
		}
		close(heredoc_pipe[1]);
		dup2(heredoc_pipe[0], STDIN_FILENO);
		close(heredoc_pipe[0]);
	}
}

void	handle_parent_process(int *prev_fd, int pipefd[2], int i, int cmds_size)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (i < cmds_size - 1)
	{
		*prev_fd = pipefd[0];
		close(pipefd[1]);
	}
}

void	wait_for_children(t_cmds *cmds, t_shell *shell)
{
	int	i;
	int	status;

	i = 0;
	while (i < cmds->size)
	{
		waitpid(cmds[i].pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
		cmds[i].exit_code = shell->exit_status;
		free_heredoc_content(&cmds[i]);
		i++;
	}
}
