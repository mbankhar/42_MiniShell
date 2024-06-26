/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 14:13:06 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/26 14:28:38 by mbankhar         ###   ########.fr       */
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
# include <ctype.h>
# include <sys/stat.h>

typedef struct s_exec
{
	int			fd_in;
	int			fd_out;
	int			number_of_pipes;
	char		**commands;
	char		**tokens;
	int			num_commands;
}			t_exec;

typedef struct s_cmds_arr
{
	char		**cmd_args;
	int			size;
}			t_cmds;

char	*altpath(char **env, char *cmd, int i);
char	*get_path(char **env, char *cmd);
void	ft_free(char **array);
int		execution(t_cmds *cmds, char **env, t_exec *exec);
int		count_char_occurrences(const char *str, char ch);
int		are_quotes_even(const char *str);
int		get_token_number(char **tokens, char **env);
void	look_for_redirect(char **commands, t_exec *exec);
void	check_dollar(char **commands, t_exec *exec);
char	**get_the_token(char **commands, t_exec *exec);
char 	*get_pathasd(char **env, char *cmd);
// char** 	parse_commands(const char* input, int* count);
char	**ft_splitspecial(char const *s, char c);
void	change_directory(char *path);
void	execute_echo(char *args[]);
void	execute_export(char *var, char *value);
void	execute_unset(char *var);
void	print_env();


#endif