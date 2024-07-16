/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 11:17:31 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/13 14:18:19 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*handle_heredoc(const char *delimiter);
// void	test_heredoc(void);
// void	execute_command_with_heredoc(char *command, char *heredoc_content);

// Add this new function


char *extract_delimiter(char *line) {
    char *delimiter_start = strstr(line, "<<");
    if (!delimiter_start) return NULL;
    delimiter_start += 2;  // Move past "<<"

    // Skip any spaces after "<<"
    while (*delimiter_start && isspace(*delimiter_start)) ++delimiter_start;

    char *delimiter_end = delimiter_start;
    while (*delimiter_end && !isspace(*delimiter_end)) ++delimiter_end;

    return strndup(delimiter_start, delimiter_end - delimiter_start);
}
void	execute_command_with_heredoc(char *command, char *heredoc_content)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	else if (pid == 0) // Child process
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		execlp(command, command, NULL);
		perror("execlp");
		exit(1);
	}
	else // Parent process
	{
		close(pipefd[0]);
		write(pipefd[1], heredoc_content, strlen(heredoc_content));
		close(pipefd[1]);
		waitpid(pid, NULL, 0);
	}
}

// Function to remove quotes from a string
char	*remove_quotess(const char *str)
{
	size_t		len;
	char		*result;
	char		*dst;
	const char	*src;

	len = strlen(str);
	result = malloc(len + 1);
	if (result == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	dst = result;
	src = str;
	while (*src != '\0')
	{
		if (*src != '\'' && *src != '\"')
		{
			*dst = *src;
			dst++;
		}
		src++;
	}
	*dst = '\0';
	return (result);
}

// Function to get the value of an environment variable from a custom environment
char	*get_custom_env_value(const char *name, char **env)
{
	size_t	name_len;
	int		i;

	name_len = ft_strlen(name);
	i = -1;
	while (env[++i] != NULL)
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
		{
			return (env[i] + name_len + 1);
		}
	}
	return (NULL);
}

// Function to expand environment variables in a string
char	*expand_env_variables(const char *str, char **env)
{
	size_t		len;
	char		*result;
	const char	*start;
	const char	*dollar;
	const char	*end;
	char		var_name[128];
	char		*value;

	len = strlen(str);
	result = malloc(len + 1);
	if (result == NULL)
		return (NULL);
	result[0] = '\0';
	start = str;
	while ((dollar = strchr(start, '$')) != NULL)
	{
		strncat(result, start, dollar - start);
		dollar++;
		end = dollar;
		while (*end && (isalnum(*end) || *end == '_'))
			end++;
		strncpy(var_name, dollar, end - dollar);
		var_name[end - dollar] = '\0';
		value = get_custom_env_value(var_name, env);
		if (value)
			strcat(result, value);
		start = end;
	}
	strcat(result, start);
	return (result);
}
void test_heredoc(void)
{
    t_heredoc_result result = handle_heredoc("EOF");
    if (result.success && result.content)
    {
        printf("Heredoc content:\n%s", result.content);
        free(result.content);
    }
    else
    {
        printf("Failed to read heredoc\n");
    }
}

// Function to handle Heredoc

t_heredoc_result handle_heredoc(const char *delimiter) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *heredoc_content = malloc(1);
    heredoc_content[0] = '\0';  // Ensuring the initial string is empty but allocated.

    if (!heredoc_content) {
        perror("Error allocating memory for heredoc content");
        return (t_heredoc_result){NULL, 0}; // Early exit on memory allocation failure
    }

    printf("Start heredoc, enter delimiter to end: '%s'\n", delimiter);

    while (1) {
        printf("heredoc> ");
        read = getline(&line, &len, stdin);  // Read line from stdin

        if (read == -1) {  // Check for read errors or EOF
            perror("getline failed");
            free(line);
            free(heredoc_content);
            return (t_heredoc_result){NULL, 0}; // Return error in heredoc reading
        }

        if (line[read - 1] == '\n') {
            line[read - 1] = '\0'; // Strip newline
        }

        if (strcmp(line, delimiter) == 0) {  // Compare line against delimiter
            printf("Delimiter '%s' found, ending heredoc.\n", delimiter);
            break;  // Exit the loop on successful delimiter match
        }

        // Append the line to heredoc content
        char *temp = realloc(heredoc_content, strlen(heredoc_content) + read + 1);
        if (!temp) {
            perror("Error reallocating memory for heredoc content");
            free(line);
            free(heredoc_content);
            return (t_heredoc_result){NULL, 0}; // Return error in heredoc reading
        }
        heredoc_content = temp;
        strcat(heredoc_content, line);
        strcat(heredoc_content, "\n");
    }

    free(line);  // Cleanup line buffer
    return (t_heredoc_result){heredoc_content, 1};  // Return successful heredoc content
}

// Function to handle redirection
void	look_for_redirect(char **commands, int index, t_cmds *cmds, char **env)
{
	int		fd;
	char	*file;
	char	*expanded_file;
 //	char	*heredoc_content;

	// int		pipefd[2];
	file = remove_quotess(commands[index + 1]);
	if (file == NULL)
		return ;
	expanded_file = expand_env_variables(file, env);
	free(file);
	if (expanded_file == NULL)
		return ;
	if (commands[index][0] == '>' && commands[index][1] == '>')
	{
		fd = open(expanded_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			perror("Failed to open file for appending");
		else
			cmds->fd_out = fd;
	}
	else if (commands[index][0] == '<' && commands[index][1] == '<')
{
    t_heredoc_result result = handle_heredoc(expanded_file);
    if (!result.success || result.content == NULL)
    {
        free(expanded_file);
        return;
    }
    execute_command_with_heredoc(commands[0], result.content);
    free(result.content);
}
	else if (commands[index][0] == '<')
	{
		fd = open(expanded_file, O_RDONLY);
		if (fd == -1)
			perror("Failed to open file for reading");
		else
			cmds->fd_in = fd;
	}
	else if (commands[index][0] == '>')
	{
		fd = open(expanded_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			perror("Failed to open file for writing");
		else
			cmds->fd_out = fd;
	}
	free(expanded_file);
}
