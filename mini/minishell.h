/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 14:13:06 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/08 13:02:34 by amohame2         ###   ########.fr       */
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
	int			number_of_pipes;
	char		**commands;
	char		**tokens;
}			t_exec;

typedef struct s_env_node {
    char *name;
    char *value;
    struct s_env_node *next;
} t_env_node;

t_env_node *g_env_list; // Global environment variable list


typedef struct s_cmds_arr {
    char    **cmd_args;
    int     size;
    int     cmd_args_count;
    int     fd_in;
    int     fd_out;
    int     pid;
} t_cmds;


char	*altpath(char **env, char *cmd, int i);
char	*get_path(char **env, char *cmd);
void	ft_free(char **array);
int		execution(t_cmds *cmds, char **env, t_exec *exec);
int		count_char_occurrences(const char *str, char ch);
int		are_quotes_even(const char *str);
int		get_token_number(char **tokens, char **env);
void	look_for_redirect(char **commands, int index, t_cmds *cmds);
void	check_dollar(char **commands, t_exec *exec);
char	**get_the_token(char **commands, t_exec *exec);
char	*get_pathasd(char **env, char *cmd);
char	**ft_splitspecial(char const *s, char c);
void	change_directory(const char *path);
void	execute_echo(char *args[]);
void	execute_export(const char *var, const char *value);
void	execute_unset(char *var);
void	print_env(void);
void	remove_quotes(char **args);
void set_env_var(const char *name, const char *value, char **environ);
void set_env_var(const char *name, const char *value, char **environ);
void add_env_node(t_env_node **env_list, const char *name, const char *value);
void update_env_node(t_env_node *env_list, const char *name, const char *value);
void execute_single_builtin(char **cmd);
void execute_builtin(t_cmds *cmds, char **environ);	
int	is_builtin_command(char *cmd);



#endif