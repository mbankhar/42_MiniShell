/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 17:00:00 by mbankhar          #+#    #+#             */
/*   Updated: 2024/08/27 22:55:27 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*read_and_expand_line(char *delimiter, char **env)
{
	char	*line;
	char	*expanded_line;

	line = readline("> ");
	if (!line || strcmp(line, delimiter) == 0)
	{
		free(line);
		return (NULL);
	}
	expanded_line = expand_env_variables(line, env);
	free(line);
	return (expanded_line);
}

static int	realloc_and_store_line(char ***content, int *line_count,
		char *expanded_line)
{
	char	**new_content;
	int		i;

	new_content = realloc(*content, (*line_count + 1) * sizeof(char *));
	if (new_content == NULL)
	{
		perror("Failed to allocate memory");
		free(expanded_line);
		i = 0;
		while (i < *line_count)
		{
			free((*content)[i]);
			i++;
		}
		free(*content);
		return (0);
	}
	*content = new_content;
	(*content)[(*line_count)++] = expanded_line;
	return (1);
}

int	handle_heredoc(char *delimiter, t_cmds *cmds, char **env)
{
	char	*expanded_line;
	char	**content;
	int		line_count;

	content = NULL;
	line_count = 0;
	printf("Enter heredoc content. End with '%s' on a new line.\n", delimiter);
	while (1)
	{
		expanded_line = read_and_expand_line(delimiter, env);
		if (!expanded_line)
			break ;
		if (!realloc_and_store_line(&content, &line_count, expanded_line))
			return (0);
	}
	cmds->heredoc_content = content;
	cmds->heredoc_line_count = line_count;
	return (1);
}

void	look_for_heredoc(char **commands, int index, t_cmds *cmds, char **env)
{
	if (commands[index + 1])
	{
		if (!handle_heredoc(commands[index + 1], cmds, env))
		{
			cmds->exit_code = 1;
		}
	}
	else
	{
		ft_putendl_fd("syntax error near unexpected token `newline'", 2);
		cmds->exit_code = 2;
	}
}

void	free_heredoc_content(t_cmds *cmd)
{
	int	i;

	if (cmd->heredoc_content)
	{
		i = 0;
		while (i < cmd->heredoc_line_count)
		{
			if (cmd->heredoc_content[i])
				free(cmd->heredoc_content[i]);
			i++;
		}
		free(cmd->heredoc_content);
		cmd->heredoc_content = NULL;
		cmd->heredoc_line_count = 0;
	}
}
