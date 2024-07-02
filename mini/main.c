/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:12:26 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/02 12:02:09 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

void sigint_handler(int signal) {
    if (signal == SIGINT) {
        printf("SIGINT received\n");
        printf("\n");
        fflush(stdout);
    }
}
void sigquit_handler(int signal) {
    if (signal == SIGQUIT) {
        printf("Quit signal received, exiting.\n");
        exit(EXIT_FAILURE);
    }
}

void sigterm_handler(int signal) {
    if (signal == SIGTERM) {
        printf("Termination signal received, exiting.\n");
        exit(EXIT_FAILURE);
    }
}

void	look_for_redirect(char **commands, int index, t_cmds *cmds)
{

	if (commands[index][0] == '>' && commands[index][1] == '>')
		cmds->fd_out
			= open(commands[index + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	// else if (commands[i][y] == '<' && commands[i][y + 1] == '<')
		// 	here_doc();
	else if (commands[index][0] == '<')
		cmds->fd_in = open(commands[index + 1], O_RDONLY);
	else if (commands[index][0] == '>')
		cmds->fd_out
			= open(commands[index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

}

void	do_shit(char **args, t_exec *exec, t_cmds **cmds_ptr)
{
	int		i;
	int		j;
	int		k;
	t_cmds	*cmds;
	 struct sigaction sa;


	i = -1;
	*cmds_ptr = malloc((exec->number_of_pipes + 1) * sizeof(t_cmds));
	cmds = *cmds_ptr;
	i = -1;
	k = 0;
	while (args[++i])
	{
		j = 0;
		while (args[i] && args[i][0] != '|')
		{
			if (args[i][0] == '<' || args[i][0] == '>')
			{
				i = i + 2;
				continue ;
			}
			else
			{
				i++;
				j++;
			}
		}
		cmds[k].cmd_args = malloc(sizeof(char *) * (j + 1));
		k++;
		if (!args[i])
			break ;
	}
	i = -1;
	k = 0;
	while (args[++i])
	{
		j = 0;
		cmds[k].fd_in = -2;
		cmds[k].fd_out = -2;
		while (args[i] && args[i][0] != '|')
		{
			if (args[i][0] == '<' || args[i][0] == '>')
			{
				look_for_redirect(args, i, &cmds[k]);
				i = i + 2;
				continue ;
			}
			else
				cmds[k].cmd_args[j++] = args[i++];
		}
		cmds[k].cmd_args[j++] = NULL;
		k++;
		if (!args[i])
		{
			break ;
		}
	}
	cmds->size = k;
}


void	remove_quotes(char **args)
{
	int		i;
	int		j;
	int		length;
	char	*temp;
	int		index;

	if (args == NULL)
		return ;
	i = -1;
	while (args[++i] != NULL)
	{
		length = strlen(args[i]);
		if (length > 1 && ((args[i][0] == '\'' && args[i][length - 1] == '\'')
			|| (args[i][0] == '\"' && args[i][length - 1] == '\"')))
		{
			temp = malloc(sizeof(char) * (length - 1));
			if (temp == NULL)
			{
				perror("malloc failed");
				exit(EXIT_FAILURE);
			}
			j = 1;
			index = 0;
			while (j < length - 1)
			{
				temp[index] = args[i][j];
				index++;
				j++;
			}
			temp[index] = '\0';
			free(args[i]);
			args[i] = temp;
		}
	}
}

bool	check_the_line(char *line, t_exec *exec, t_cmds *cmds)
{
	int			count;
	bool        should_continue = true;
	char		**args;
	extern char	**environ;

	count = 0;
	if (are_quotes_even(line) != 0)
	{
		exec->number_of_pipes = count_char_occurrences(line, '|');
		check_dollar(&line, exec);
		args = ft_splitspecial(line, '|');
		do_shit(args, exec, &cmds);
		exec->tokens = get_the_token(args, exec);
		
		if (cmds->size > 0 && is_builtin_command(cmds[0].cmd_args[0])) {
			execute_builtin(cmds[0].cmd_args);
		} else {
			execution(cmds, environ, exec);
		}
		
		if (cmds->size > 0 && strcmp(cmds[0].cmd_args[0], "exit") == 0)
			should_continue = false;
		
		ft_free(args);
	}
	else
		printf("Quotes error\n");
	return should_continue;
}
int main(int argc, char **argv, char **envp) {
    char *line;
    t_exec exec;
    t_cmds *cmds;
    struct sigaction sa;

    (void)argc;
    (void)argv;
    (void)envp;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigint_handler;
    sigaction(SIGINT, &sa, NULL);

    // Set default PATH if not set
    if (getenv("PATH") == NULL) {
        setenv("PATH", "/usr/bin:/bin", 1);
    }

    while (1) {
        line = readline("minishell> ");
        if (!line)
            break;
        if (strlen(line) > 0) {
            add_history(line);
            if (!check_the_line(line, &exec, cmds))
                break;
        }
        free(line);
    }
    return 0;
}

void	look_for_builtin(t_cmds *cmds, t_exec *exec, char **environ)
{
	if (strcmp(cmds[0].cmd_args[0], "cd") == 0)
	{
		if (cmds[0].cmd_args[1] == NULL)
			fprintf(stderr, "cd: missing argument\n");
		else
			change_directory(cmds[0].cmd_args[1]);
	}
	else if (strcmp(cmds[0].cmd_args[0], "echo") == 0)
		execute_echo(cmds[0].cmd_args);
	else if (strcmp(cmds[0].cmd_args[0], "export") == 0)
	{
		if (cmds[0].cmd_args[1] == NULL)
			fprintf(stderr, "export: missing argument\n");
		else
		{
			char *var = strtok(cmds[0].cmd_args[1], "=");
			char *value = strtok(NULL, "=");
			if (value != NULL)
				execute_export(var, value);
			else if (value == NULL && ft_strchr(cmds[0].cmd_args[1], "=" != NULL))
				execute_export(var, "");
		}
	}
	else if (strcmp(cmds[0].cmd_args[0], "unset") == 0)
	{
		if (cmds[0].cmd_args[1] == NULL)
			fprintf(stderr, "unset: missing argument\n");
		else
			execute_unset(cmds[0].cmd_args[1]);
	}
	else if (strcmp(cmds[0].cmd_args[0], "env") == 0)
		print_env();
	// else if (strcmp(cmds[0].cmd_args[0], "exit") == 0)
		// exit(1);
	else
		execution(cmds, environ, exec);
}
