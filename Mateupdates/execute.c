/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:18:10 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/25 15:56:01 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>


void execute(char **env, char **cmd, t_shell *shell);

int	is_builtin(t_cmds *cmds)
{
	if (ft_strcmp(cmds[0].cmd_args[0], "cd") == 0)
		return (1);
	else if (ft_strcmp(cmds[0].cmd_args[0], "echo") == 0)
		return (1);
	else if (ft_strcmp(cmds[0].cmd_args[0], "export") == 0)
		return (1);
	else if (ft_strcmp(cmds[0].cmd_args[0], "unset") == 0)
		return (1);
	else if (ft_strcmp(cmds[0].cmd_args[0], "env") == 0)
		return (1);
	else if (ft_strcmp(cmds[0].cmd_args[0], "pwd") == 0)
		return (1);
	else
		return (0);
}

int handle_redirections(char **cmd, t_cmds *cmds, char **env, t_shell *shell)
{
    int i = 0;

  //  printf("Debug: Entering handle_redirections\n");

    while (cmd[i] != NULL)
    {
     //printf("Debug: Processing cmd[%d]: %s\n", i, cmd[i]);
        if (cmd[i][0] == '<' || cmd[i][0] == '>')
        {
            look_for_redirect(cmd, i, cmds, env, shell);
            if (shell->exit_status == 1)
            {
                return 1;
            }
            i += 2;
        }
        else
        {
            i++;
        }
    }

  //printf("Debug: Exiting handle_redirections\n");
    return 0;
}

int execute_builtin(t_cmds *cmds, char ***env, t_shell *shell)
{
   if (ft_strcmp(cmds[0].cmd_args[0], "cd") == 0)
    {
        // Ignore extra arguments for cd
        return change_directory(cmds[0].cmd_args[1], env, shell);
    }
    else if (ft_strcmp(cmds[0].cmd_args[0], "echo") == 0)
        return execute_echo(cmds, *env, shell);
    else if (ft_strcmp(cmds[0].cmd_args[0], "export") == 0)
        return execute_export(cmds, env);
    else if (ft_strcmp(cmds[0].cmd_args[0], "unset") == 0)
        return execute_unset(cmds, env);
    else if (ft_strcmp(cmds[0].cmd_args[0], "env") == 0)
    {
        print_env(*env);
        return 0;
    }
     else if (ft_strcmp(cmds[0].cmd_args[0], "pwd") == 0)
    {
        return execute_pwd(cmds[0].cmd_args);
    }
    return 1;
}
// Execute a single command
// void	execute(char **env, char **cmd, t_shell *shell);

int	*get_exit(void)
{
	static int	exitcode = 0;

	return (&exitcode);
}


int execution(t_cmds *cmds, char ***env, t_shell *shell)
{
    int pipefd[2];
    int prev_fd;
    int i;
    int status;
    int heredoc_pipe[2];

    prev_fd = -1;
    i = -1;
    if (cmds->cmd_number == 1 && is_builtin(cmds))
    {
        shell->exit_status = execute_builtin(cmds, env, shell);
        cmds->exit_code = shell->exit_status;
        //Debug: Builtin command executed, exit_code: 0printf("Debug: Builtin command executed, exit_code: %d\n", cmds->exit_code); // Debugging line
        return shell->exit_status;
    }
    else
    {
        while (++i < cmds->size)
        {
            if (i < cmds->size - 1 && pipe(pipefd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
            cmds[i].pid = fork();
            if (cmds[i].pid == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            if (cmds[i].pid == 0)
            {
                if (i == 0)
                {
                    if (cmds[i].fd_in != -2)
                        dup2(cmds[i].fd_in, STDIN_FILENO);
                    if (cmds[i].fd_out != -2)
                        dup2(cmds[i].fd_out, STDOUT_FILENO);
                    else
                        dup2(pipefd[1], STDOUT_FILENO);
                }
                else if (i == cmds->size - 1)
                {
                    if (cmds[i].fd_in != -2)
                        dup2(cmds[i].fd_in, STDIN_FILENO);
                    else
                        dup2(prev_fd, STDIN_FILENO);
                    if (cmds[i].fd_out != -2)
                        dup2(cmds[i].fd_out, STDOUT_FILENO);
                }
                else
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
                if (cmds[i].fd_in != -2)
                    close(cmds[i].fd_in);
                if (cmds[i].fd_out != -2)
                    close(cmds[i].fd_out);
                if (prev_fd != -1)
                    close(prev_fd);
                if (i < cmds->size - 1)
                {
                    close(pipefd[0]);
                    close(pipefd[1]);
                }
                if (cmds[i].heredoc_content)
                {
                    if (pipe(heredoc_pipe) == -1)
                    {
                        perror("pipe");
                        exit(EXIT_FAILURE);
                    }
                    for (int j = 0; j < cmds[i].heredoc_line_count; j++)
                    {
                        if (cmds[i].heredoc_content[j])
                        {
                            write(heredoc_pipe[1], cmds[i].heredoc_content[j],
                                strlen(cmds[i].heredoc_content[j]));
                            write(heredoc_pipe[1], "\n", 1);
                        }
                    }
                    close(heredoc_pipe[1]);
                    dup2(heredoc_pipe[0], STDIN_FILENO);
                    close(heredoc_pipe[0]);
                }
                execute(*env, cmds[i].cmd_args, shell);
                exit(EXIT_SUCCESS);
            }
            else
            {
                if (prev_fd != -1)
                    close(prev_fd);
                if (i < cmds->size - 1)
                {
                    prev_fd = pipefd[0];
                    close(pipefd[1]);
                }
            }
        }
        i = -1;
        while (++i < cmds->size)
        {
            waitpid(cmds[i].pid, &status, 0);
            if (WIFEXITED(status))
                shell->exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                shell->exit_status = 128 + WTERMSIG(status);
            cmds[i].exit_code = shell->exit_status; // Update cmds->exit_code
          //  printf("Debug: Command executed, exit_code: %d\n", cmds[i].exit_code); // Debugging line
            free_heredoc_content(&cmds[i]);
        }
    }
    return shell->exit_status;
}


void execute(char **env, char **cmd, t_shell *shell)
{
    char *path_cmd;
    t_cmds cmds; // Declare cmds

    // Initialize cmds
    cmds.fd_in = -1;
    cmds.fd_out = -1;

    // Handle redirections
    if (handle_redirections(cmd, &cmds, env, shell) == 1)
    {
        shell->exit_status = 1;
        return;
    }

    // Check if the command is empty or $EMPTY
    if (cmd[0] == NULL || cmd[0][0] == '\0' || strcmp(cmd[0], "$EMPTY") == 0)
    {
        // If $EMPTY is followed by another command, shift the command array
        if (cmd[1] != NULL)
        {
            cmd = &cmd[1];
        }
        else
        {
            shell->exit_status = 0;
            return;
        }
    }

    expand_in_2darray(&cmd, env);
    remove_quotes(cmd);

    // Check again if the command is empty after expansion and quote removal
    if (cmd[0] == NULL || cmd[0][0] == '\0')
    {
        shell->exit_status = 0;
        return;
    }

    path_cmd = get_path(env, cmd[0]);
    if (!path_cmd)
    {
        fprintf(stderr, "minishell: %s: command not found\n", cmd[0]);
        shell->exit_status = 127;
        exit(127);
    }
    else
    {
        if (execve(path_cmd, cmd, env) == -1)
        {
            perror("minishell");
            if (errno == ENOENT)
            {
                shell->exit_status = 127;
                exit(127);
            }
            else
            {
                shell->exit_status = 126;
                exit(126);
            }
        }
    }
}