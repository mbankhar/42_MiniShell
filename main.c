/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:12:26 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/18 17:01:43 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

// Function prototypes
char** parse_commands(const char* input, int* count);
int is_redirection(const char* token);


char** parse_commands(const char* input, int* count) {
    char* input_copy = strdup(input); // Make a modifiable copy of the input
    char* token;
    char** result = NULL;
    int num_commands = 0;
    int within_quotes = 0;
    int is_last_token_redirection = 0;
    size_t command_length = 0;

    // Allocate initial memory for result array
    result = malloc(sizeof(char*));
    result[0] = malloc(1);
    result[0][0] = '\0';

    // Tokenize based on spaces to handle each element
    token = strtok(input_copy, " ");
    while (token != NULL) {
        // Check if token contains quotes
        if (strchr(token, '\"') != NULL) {
            within_quotes = !within_quotes;
        }

        // If the previous token was a redirection operator, skip this token
        if (is_last_token_redirection) {
            is_last_token_redirection = 0;
            token = strtok(NULL, " ");
            continue;
        }

        // If the token is a redirection operator, skip it and the next token
        if (is_redirection(token)) {
            is_last_token_redirection = 1;
            token = strtok(NULL, " ");
            continue;
        }

        // Calculate the required length for the command
        command_length = strlen(result[num_commands]) + strlen(token) + 2;

        // Reallocate memory for the current command
        result[num_commands] = realloc(result[num_commands], command_length);

        // Append token to the last command in result array
        if (within_quotes || strchr(token, '|') == NULL) {
            if (result[num_commands][0] != '\0') {
                strcat(result[num_commands], " ");
            }
            strcat(result[num_commands], token);
        } else {
            // Split by pipe and start a new command
            num_commands++;
            result = realloc(result, (num_commands + 1) * sizeof(char*));
            result[num_commands] = malloc(strlen(token) + 1);
            strcpy(result[num_commands], token + 1); // Skip the pipe character
        }

        token = strtok(NULL, " ");
    }

    // Trim leading spaces from each command
    for (int i = 0; i <= num_commands; i++) {
        char* cmd = result[i];
        while (*cmd == ' ') cmd++;
        memmove(result[i], cmd, strlen(cmd) + 1);
    }

    free(input_copy);
    *count = num_commands + 1;
    return result;
}

int is_redirection(const char* token) {
    return (strcmp(token, "<") == 0 || strcmp(token, ">") == 0 || 
            strcmp(token, "<<") == 0 || strcmp(token, ">>") == 0);
}


void	check_the_line(char *line, t_exec *exec)
{
	int			i;
	int			count;
	extern char	**environ;

	count = 0;
	if (are_quotes_even(line) != 0)
	{
		exec->number_of_pipes = count_char_occurrences(line, '|');
		exec->commands = ft_split(line, ' ');
		look_for_redirect(exec->commands, exec);
		check_dollar(exec->commands, exec);
		exec->tokens = get_the_token(exec->commands, exec);
		exec->num_commands = 0;
		exec->num_commands = get_token_number(exec->tokens, environ);
		exec->commands_parsed = parse_commands(line, &count);
		// i = -1;
		// while (exec->commands_parsed[++i])
		// 	printf("%s\n", exec->commands_parsed[i]);
		// i = -1;
		// while (exec->tokens[++i])
		// 	printf("%s\n", exec->tokens[i]);
		// exit(1);
		execution(exec->commands_parsed, environ, exec);
	}
	else
	{
		printf("Quotes error\n");
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	input[MAX_INPUT_SIZE];
	char	*args[MAX_ARG_SIZE];
	t_exec	exec;

	(void)argc;
	(void)argv;
	(void)envp;
	exec.fd_in = -2;
	exec.fd_out = -2;
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break ;
		if (strlen(line) > 0)
		{
			add_history(line);
			check_the_line(line, &exec);
		}
		free(line);
	}
	return (0);
}


// int	main(void)
// {
// 	char	input[MAX_INPUT_SIZE];
// 	char	*args[MAX_ARG_SIZE];
// 	char	*token;
// 	int		should_run = 1;
// 	char		**env;
// 	t_exec	exec;

// 	while (should_run)
// 	{
// 		printf("minishell> ");
// 		fflush(stdout);
// 		if (!fgets(input, MAX_INPUT_SIZE, stdin))
// 			break ;
// 		input[strcspn(input, "\n")] = 0;
// 		int arg_count = 0;
// 		token = strtok(input, " ");
// 		while (token != NULL && arg_count < MAX_ARG_SIZE - 1)
// 		{
// 			args[arg_count++] = token;
// 			token = strtok(NULL, " ");
// 		}
// 		args[arg_count] = NULL;
// 		if (args[0] == NULL)
// 			continue ;
// 		else if (pipes != 0)
// 			execution(pipes);
// 		else if (strcmp(args[0], "cd") == 0)
// 		{
// 			if (args[1] == NULL)
// 				fprintf(stderr, "cd: missing argument\n");
// 			else
// 				change_directory(args[1]);
// 		}
// 		else if (strcmp(args[0], "echo") == 0)
// 			execute_echo(args);
// 		else if (strcmp(args[0], "export") == 0)
// 		{
// 			if (args[1] == NULL)
// 				fprintf(stderr, "export: missing argument\n");
// 			else
// 			{
// 				char *var = strtok(args[1], "=");
// 				char *value = strtok(NULL, "=");
// 				if (value != NULL)
// 					execute_export(var, value);
// 				else if (value == NULL && ft_strchr(args[1], "=" != NULL))
// 					execute_export(var, "");
// 			}
// 		}
// 		else if (strcmp(args[0], "unset") == 0)
// 		{
// 			if (args[1] == NULL)
// 				fprintf(stderr, "unset: missing argument\n");
// 			else
// 				execute_unset(args[1]);
// 		}
// 		else if (strcmp(args[0], "env") == 0)
// 			print_env();
// 		else if (strcmp(args[0], "exit") == 0)
// 			should_run = 0;
// 		else
// 			execute_command(args);
// 	}
// 	return (0);
// }
