/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 17:00:00 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/18 16:27:20 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static char	*get_heredoc_filename(void)
// {
// 	static int	count = 0;
// 	char		*filename;
// 	char		*number;

// 	number = ft_itoa(count++);
// 	filename = ft_strjoin("/tmp/.heredoc_", number);
// 	free(number);
// 	return (filename);
// }

// static void	write_to_heredoc(int fd, char *delimiter, char **env)
// {
// 	char	*line;
// 	char	*expanded_line;

// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line || ft_strcmp(line, delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		expanded_line = expand_env_variables(line, env);
// 		ft_putendl_fd(expanded_line, fd);
// 		free(expanded_line);
// 		free(line);
// 	}
// }

// int handle_heredoc(char *delimiter, t_cmds *cmds, char **env)
// {
//     char    *filename;
//     int     fd;
//     char    *line;
//     int     line_count = 0;
//     char    **content = NULL;

//     filename = get_heredoc_filename();
//     fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd == -1)
//     {
//         perror("Failed to create heredoc file");
//         free(filename);
//         return (0);
//     }
//     write_to_heredoc(fd, delimiter, env);
//     close(fd);

//     fd = open(filename, O_RDONLY);
//     if (fd == -1)
//     {
//         perror("Failed to open heredoc file for reading");
//         unlink(filename);
//         free(filename);
//         return (0);
//     }

//     while ((line = get_next_line(fd)) != NULL)
//     {
//         char *expanded_line = expand_env_variables(line, env);
//         content = realloc(content, (line_count + 1) * sizeof(char *));
//         if (content == NULL)
//         {
//             perror("Failed to allocate memory");
//             free(expanded_line);
//             free(line);
//             close(fd);
//             unlink(filename);
//             free(filename);
//             return (0);
//         }
//         content[line_count++] = expanded_line;
//         free(line);  // Free the original line after expansion
//     }

//     close(fd);
//     unlink(filename);
//     free(filename);

//     cmds->heredoc_content = content;
//     cmds->heredoc_line_count = line_count;
//     return (1);
// }



int handle_heredoc(char *delimiter, t_cmds *cmds, char **env)
{
    char *line;
    char **content = NULL;
    int line_count = 0;

    printf("Enter heredoc content. End with '%s' on a new line.\n", delimiter);

    while (1)
    {
        line = readline("> ");
        if (!line || strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }

        char *expanded_line = expand_env_variables(line, env);
        content = realloc(content, (line_count + 1) * sizeof(char *));
        if (content == NULL)
        {
            perror("Failed to allocate memory");
            free(expanded_line);
            free(line);
            for (int i = 0; i < line_count; i++)
                free(content[i]);
            free(content);
            return (0);
        }
        content[line_count++] = expanded_line;
        free(line);
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

void free_heredoc_content(t_cmds *cmd)
{
    if (cmd->heredoc_content)
    {
        for (int i = 0; i < cmd->heredoc_line_count; i++)
        {
            if (cmd->heredoc_content[i])
                free(cmd->heredoc_content[i]);
        }
        free(cmd->heredoc_content);
        cmd->heredoc_content = NULL;
        cmd->heredoc_line_count = 0;
    }
}