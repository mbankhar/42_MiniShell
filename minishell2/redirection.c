/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 11:17:31 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/17 14:19:35 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*handle_heredoc(const char *delimiter);
// void	test_heredoc(void);
// void	execute_command_with_heredoc(char *command, char *heredoc_content);

// Add this new function

char	*get_temp_filename(void)
{
	static int	count = 0;
	char		*filename;

	filename = malloc(20);
	if (!filename)
		return (NULL);
	snprintf(filename, 20, "/tmp/heredoc_%d", count++);
	return (filename);
}
int	handle_heredoc(const char *delimiter)
{
	char	*line;
	char	*temp_file;
	int		fd;

	temp_file = get_temp_filename();
	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("Error opening temporary file");
		free(temp_file);
		return (-1);
	}
	printf("Enter heredoc content. End with '%s' on a new line.\n", delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (open(temp_file, O_RDONLY));
}

void	execute_command(char *command, int input_fd)
{
	pid_t	pid;
		char *args[] = {"/bin/sh", "-c", command, NULL};
		int status;

	pid = fork();
	if (pid == 0)
	{
		// Child process
		if (input_fd != STDIN_FILENO)
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		execve("/bin/sh", args, NULL);
		perror("execve");
		exit(1);
	}
	else if (pid > 0)
	{
		// Parent process
		waitpid(pid, &status, 0);
	}
	else
	{
		perror("fork");
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


// Function to handle redirection
void	look_for_redirect(char **commands, int index, t_cmds *cmds, char **env)
{
	int					fd;
	char				*file;
	char				*expanded_file;
	// t_heredoc_result	result;

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
