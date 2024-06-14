/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 14:13:06 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/14 11:40:21 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# define MAX_COMMANDS 16
# define MAX_TOKENS 128
# define MAX_FLAGS 32

typedef enum {
    CMD_NORMAL,
    CMD_PIPE,
    CMD_REDIRECT_IN,
    CMD_REDIRECT_OUT,
    CMD_REDIRECT_APPEND
} CommandType;

typedef struct {
    char **tokens;
    char **flags;
    int num_flags;
    CommandType type;
    char *redirect_file;
    int input_fd;
    int output_fd;
} SimpleCommand;

typedef struct {
    SimpleCommand commands[MAX_COMMANDS];
    int num_commands;
} Command;

char *altpath(char **env, char *cmd, int i);
char *get_path(char **env, char *cmd);
void ft_free(char **array);

// Function prototypes for parsing and execution
Command *parse_input(char *line);
void free_command(Command *cmd);
void expand_variables_in_tokens(char **tokens);
void execute_command(Command *cmd);
int count_pipes(const char *line);

#endif
