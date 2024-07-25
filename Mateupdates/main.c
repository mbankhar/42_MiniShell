/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:12:26 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/25 12:28:23 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int	handle_pipe_error(char last, char *line, int *i)
{
    if (strncmp(line, "echo", 4) == 0 && isspace(line[4]))
    {
        int j = 4;
        while (isspace(line[j]))
            j++;
        
        if (*i >= j)
            return (0);
    }

    if (last == '|' || last == '\0')
    {
        printf("Error: Invalid pipe usage.\n");
        return (1);
    }
    return (0);
}

int	handle_output_redirection(char *line, int *i, char last)
{
    if (strncmp(line, "echo", 4) == 0 && isspace(line[4]))
    {
        int j = 4;
        while (isspace(line[j]))
            j++;
        
        if (*i >= j)
            return (0);
    }

    if (line[*i + 1] == '>')
    {
        if (last == '>' || last == '\0')
        {
            printf("Error: Invalid append redirection.\n");
            return (1);
        }
        (*i)++;
    }
    else
    {
        if (last == '>' || last == '\0')
        {
            printf("Error: Invalid output redirection.\n");
            return (1);
        }
    }
    return (0);
}

int	handle_input_redirection(char *line, int *i, char last)
{
	if (line[*i + 1] == '<') // Check for heredoc
	{
		(*i)++;   // Move past the second '<'
		return (0); // Heredoc is valid, so return 0
	}
	if (last == '<' || last == '\0')
	{
		printf("Error: Invalid input redirection.\n");
		return (1);
	}
	return (0);
}

int	error_check_end_conditions(char last_char, int in_pipe, int in_redirection,
		int out_redirection)
{
	if (last_char == '|' || last_char == '>' || last_char == '<')
	{
		printf("Error: Incomplete command.\n");
		return (1);
	}
	if (in_pipe || in_redirection || out_redirection)
	{
		printf("Error: Improper command ending.\n");
		return (1);
	}
	return (0);
}

int	handle_special_characters(char current, char last_char, int *in_pipe,
		int *in_redirection, int *out_redirection, int *i, char *line)
{
	if (current == '|')
	{
		if (handle_pipe_error(last_char, line, i))
			return (1);
		*in_pipe = 1;
	}
	else if (current == '>')
	{
		if (handle_output_redirection(line, i, last_char))
			return (1);
		*out_redirection = 1;
	}
	else if (current == '<')
	{
		if (handle_input_redirection(line, i, last_char))
			return (1);
		*in_redirection = 1;
	}
	else
	{
		if (*in_pipe && current != ' ')
			*in_pipe = 0;
		if (*in_redirection && current != ' ')
			*in_redirection = 0;
		if (*out_redirection && current != ' ')
			*out_redirection = 0;
	}
	return (0);
}

int	check_syntax(char *line, int len, int *in_pipe, int *in_redirection,
		int *out_redirection)
{
	char	last_char;

	last_char = '\0';
	for (int i = 0; i < len; i++)
	{
		if (isspace(line[i]))
			continue ;
		if (handle_special_characters(line[i], last_char, in_pipe,
				in_redirection, out_redirection, &i, line))
			return (1);
		last_char = line[i];
	}
	return (error_check_end_conditions(last_char, *in_pipe, *in_redirection,
			*out_redirection));
}

int	error_check_on_pipe_and_redirection(char *line)
{
	int	len;
	int	in_redirection;
	int	out_redirection;
	int	in_pipe;

	len = strlen(line);
	in_redirection = 0;
	out_redirection = 0;
	in_pipe = 0;
	return (check_syntax(line, len, &in_pipe, &in_redirection,
			&out_redirection));
}
int	is_special_char(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == ';' || c == '&');
}

int	is_quoted(char *str, int index)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	for (int i = 0; i < index; i++)
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
	}
	return (in_single_quote || in_double_quote);
}

void check_the_line(char *line, t_exec *exec, t_cmds *cmds, char ***environ, t_shell *shell)
{
    char **args;

    if (are_quotes_even(line) != 0 && !redirection_error_checks(line)
        && !error_check_on_pipe_and_redirection(line))
    {
        exec->number_of_pipes = count_char_occurrences(line, '|');
        args = ft_splitspecial(line);
        exec->tokens = get_the_token(args);
        do_shit(args, exec, &cmds, *environ, shell);
        cmds->exit_code = 0; // Ensure exit_code is initialized
        count_commands(cmds, *environ);
        if (exec->number_of_pipes == cmds->cmd_number
            && (exec->number_of_pipes != 0))
            printf("Not nice error\n");
        else
            shell->exit_status = execution(cmds, environ, shell);
     //   printf("Debug: After execution, exit_status: %d\n", shell->exit_status); // Debugging line
        for (int i = 0; i < exec->number_of_pipes + 1; i++)
        {
            free_heredoc_content(&cmds[i]);
        }
        ft_free(args);
    }
    else
    {
     //   printf(" syntax error near unexpected token\n");
        shell->exit_status = 2; // Set exit status for syntax error
      //  printf("Debug: Syntax error, exit_status: %d\n", shell->exit_status); // Debugging line
    }
}

int		g_variable = 0;

int main(void)
{
    extern char **environ;
    char *line;
    t_exec exec;
    t_cmds *cmds;
    char **args;
    char **environ_copy;
    t_shell shell;

    cmds = NULL;
    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_sigtstp);
    signal(SIGQUIT, handle_sigquit);
    if (environ[0] == NULL)
        environ = init_env(environ);
    else
        environ_copy = duplicate_environ(environ);

    shell.exit_status = 0;

    while (1)
    {
        line = readline("minishell> ");
        if (!line)
            break;
        if (strlen(line) > 0)
        {
            add_history(line);
            args = ft_splitspecial(line);
            if (args && args[0] && ft_strcmp(args[0], "exit") == 0)
            {
                int exit_status = handle_exit(args);
                if (exit_status == 1) // If not "too many arguments"
                {
                    shell.exit_status = 1;
                }
                else
                {
                    ft_free(args);
                    free(line);
                    exit(exit_status);
                }
            }
            else
            {
                check_the_line(line, &exec, cmds, &environ_copy, &shell);
            }
           // printf("Debug: After check_the_line, exit_status: %d\n", shell.exit_status); // Debugging line
            ft_free(args);
        }
        free(line);
    }
    return shell.exit_status;
}