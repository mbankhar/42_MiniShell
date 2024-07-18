/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 14:13:06 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/18 16:26:39 by amohame2         ###   ########.fr       */
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
# include <signal.h>


#define MAX_DELIMITER_LENGTH 256

typedef struct s_exec
{
	int			number_of_pipes;
	char		**commands;
	char		**tokens;
	int			num_commands;
}			t_exec;

typedef struct s_cmds_arr
{
	char		**cmd_args;
	int			size;
	int			cmd_number;
	// int			cmd_args_count;
	int			fd_in;
	int			fd_out;
	int			pid;
	int			exit_code;
    char    **heredoc_content;  // This should be a double pointer
		    int     heredoc_line_count;

}	t_cmds;

extern int	g_variable;


// Functions for parsing
int		count_char_occurrences(const char *str, char ch);
char	**get_the_token(char **commands);
int		get_token_number(char **tokens, char **env);
int		are_quotes_even(const char *str);
void	look_for_redirect(char **commands, int index, t_cmds *cmds, char **env);
char	**ft_splitspecial(char const *s);
void	do_shit(char **args, t_exec *exec, t_cmds **cmds_ptr, char **env);
char	**init_env(char **env);
int		redirection_error_checks(char *line);
char	**duplicate_environ(char **str);
void	count_commands(t_cmds *cmds, char **env);
void	look_for_redirect(char **commands, int index, t_cmds *cmds, char **env);
// Builtin functions
void	execute_echo(t_cmds *cmds, char **env);
void	execute_export(t_cmds *cmds, char ***env);
void	execute_unset(t_cmds *cmds, char ***env);
void	change_directory(char *path, char ***env);
void	print_env(char **env);
void	pwd(char **env);
// Execution functions
char	*altpath(char **env, char *cmd, int i);
char	*get_path(char **env, char *cmd);
int		execution(t_cmds *cmds, char ***env);
void	remove_quotes(char **args);
void	expand_in_2darray(char ***cmd, char **env);
char	*get_env_values(char **env, const char *var);
int		is_builtin(t_cmds *cmds);
// Additional functions
void	ft_free(char **array);
int		ft_strcmp(const char *s1, const char *s2);
// Signals
void	handle_sigint(int sig);
void	handle_sigtstp(int sig);
void	handle_sigquit(int sig);
int     handle_heredoc(char *delimiter, t_cmds *cmds, char **env);
void    look_for_heredoc(char **commands, int index, t_cmds *cmds, char **env);
int     handle_input_redirection(char *line, int *i, char last);
void    execute(char **env, char **cmd);
char	*expand_env_variables(const char *str, char **env);
void free_heredoc_content(t_cmds *cmds);


#endif