/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 14:13:06 by mbankhar          #+#    #+#             */
/*   Updated: 2024/08/28 20:56:49 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <ctype.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_shell
{
	int			exit_status;
}				t_shell;

# define MAX_DELIMITER_LENGTH 256

typedef struct s_exec
{
	int			number_of_pipes;
	char		**commands;
	char		**tokens;
	int			num_commands;
}				t_exec;

typedef struct s_cmds_arr
{
	char		**cmd_args;
	int			size;
	int			cmd_number;
	int			fd_in;
	int			fd_out;
	int			pid;
	int			exit_code;
	char		**heredoc_content;
	int			heredoc_line_count;
}				t_cmds;

typedef struct s_redirect_data
{
	char		**commands;
	int			index;
	t_cmds		*cmds;
	char		**env;
	t_shell		*shell;
}				t_redirect_data;

typedef struct s_handle_line_data
{
	char		*line;
	t_exec		*exec;
	t_cmds		*cmds;
	char		***environ;
	t_shell		*shell;
}				t_handle_line_data;

typedef struct s_do_shit_data
{
	char		**args;
	t_exec		*exec;
	t_cmds		**cmds_ptr;
	char		**env;
	t_shell		*shell;
}				t_do_shit_data;

typedef struct s_check_line_data
{
	char		*line;
	t_exec		*exec;
	t_cmds		*cmds;
	char		***environ;
	t_shell		*shell;
}				t_check_line_data;

typedef struct s_special_char_data
{
	char		current;
	char		last_char;
	int			*in_pipe;
	int			*in_redirection;
	int			*out_redirection;
	int			*i;
	char		*line;
}				t_special_char_data;

typedef struct s_check_syntax_data
{
	char		*line;
	int			len;
	int			*in_pipe;
	int			*in_redirection;
	int			*out_redirection;
}				t_check_syntax_data;

typedef struct s_echo_helper_data
{
	t_cmds		*cmds;
	char		**env;
	t_shell		*shell;
	int			original_stdout;
	int			original_stdin;
}				t_echo_helper_data;

typedef struct s_redirect_helper_data
{
	char		**cmd;
	t_cmds		*cmds;
	char		**env;
	t_shell		*shell;
}				t_redirect_helper_data;

typedef struct s_echo_data
{
	t_cmds		*cmds;
	char		**env;
	t_shell		*shell;
	int			*i;
	int			*first_arg;
}				t_echo_data;

typedef struct s_echo_helper_vars
{
	int			i;
	int			newline;
	int			first_arg;
	int			original_stdout;
	int			original_stdin;
	t_echo_data	data;
}				t_echo_helper_vars;

typedef struct s_expand_vars
{
	char		*str;
	char		*dollar_pos;
	char		var_name[256];
	char		new_str[1024];
	char		*env_value;
}				t_expand_vars;

typedef struct s_exec_params
{
	t_cmds		*cmds;
	char		***env;
	t_shell		*shell;
	int			*prev_fd;
	int			pipefd[2];
	int			index;
}				t_exec_params;

typedef struct s_altpath_vars
{
	char		**realpath;
	char		*string;
	int			i;
}				t_altpath_vars;

typedef struct s_quote_state
{
	int			in_double_quotes;
	int			in_single_quotes;
	int			inword;
	int			wordcount;
}				t_quote_state;

typedef struct s_exec_params_local
{
	t_cmds		*cmds;
	char		***env;
	t_shell		*shell;
	int			*prev_fd;
	int			pipefd[2];
	int			i;
}				t_exec_params_local;

typedef struct s_update_var_params
{
	char		***env;
	int			idx;
	const char	*var;
	const char	*value;
	int			append;
}				t_update_var_params;

typedef struct s_var_params
{
	char		*arg;
	char		*var;
	char		**value;
	int			*append;
}				t_var_params;

void			set_g_variable(int value);
// Functions for parsing
int				count_char_occurrences(const char *str, char ch);
char			**get_the_token(char **commands);
int				get_token_number(char **tokens, char **env);
int				are_quotes_even(const char *str);
char			**ft_splitspecial(char const *s);
void			do_shit(t_do_shit_data *data);
char			**init_env(char **env);
int				redirection_error_checks(char *line);
char			**duplicate_environ(char **str);
void			count_commands(t_cmds *cmds, char **env);
void			look_for_redirect(t_redirect_data *data);

// Builtin functions
int				execute_echo(t_cmds *cmds, char **env, t_shell *shell);
int				execute_export(t_cmds *cmds, char ***env);
int				execute_unset(t_cmds *cmds, char ***env);
int				change_directory(char *path, char ***env, t_shell *shell);
void			print_env(char **env);
int				execute_pwd(char **cmd_args);

// Execution functions
char			*altpath(char **env, char *cmd, int i);
char			*get_path(char **env, char *cmd);
int				execution(t_cmds *cmds, char ***env, t_shell *shell);
void			remove_quotes(char **args);
void			expand_in_2darray(char ***cmd, char **env);
char			*get_env_values(char **env, const char *var);
int				is_builtin(t_cmds *cmds);

// Additional functions
void			ft_free(char **array);
int				ft_strcmp(const char *s1, const char *s2);
void			handle_sigint(int sig);
void			handle_sigtstp(int sig);
void			handle_sigquit(int sig);
int				handle_heredoc(char *delimiter, t_cmds *cmds, char **env);
void			look_for_heredoc(char **commands, int index, t_cmds *cmds,
					char **env);
int				handle_input_redirection(char *line, int *i, char last);
void			execute(char **env, char **cmd, t_shell *shell);
char			*expand_env_variables(const char *str, char **env);
void			free_heredoc_content(t_cmds *cmds);
char			*ft_strjoin_free(char *s1, char *s2);
char			*ft_strjoin_free_char(char *s, char c);
char			*strip_quotes(char *str);
char			*ft_strndup(const char *s, size_t n);
size_t			ft_strnlen(const char *s, size_t maxlen);
int				handle_exit(char **args);
int				is_valid_exit_arg(const char *arg);
long long		ft_atoll(const char *str);
char			*trim_quotes(char *str);
char			*trim_quotes_and_spaces(char *str);
int				is_valid_identifier(const char *str);
void			remove_env_var(char ***env, const char *var);
int				handle_redirections(char **cmd, t_cmds *cmds, char **env,
					t_shell *shell);
int				is_builtin_count(char *cmd);

char			*strip_quotes_helper(char *str);
char			*itoa_helper(int num);
char			*expand_variable_helper(char *str, char **env, t_shell *shell);
int				execute_echo_helper(t_cmds *cmds, char **env, t_shell *shell);
void			print_env_helper(char **env);
int				execute_pwd_helper(char **cmd_args);
char			*ft_strcat_helper(char *dest, const char *src);
int				find_env_index_helper(char **env, const char *var);
void			update_env_var_helper(char ***env, char *var, char *value);
char			*find_oldpwd_helper(char ***env);
char			*get_home_dir_helper(char **env);
char			*expand_variable(char *str, char **env, t_shell *shell);
int				is_builtin_helper(t_cmds *cmds);
int				handle_redirections_helper(char **cmd, t_cmds *cmds, char **env,
					t_shell *shell);
int				execute_builtin_helper(t_cmds *cmds, char ***env,
					t_shell *shell);
int				*get_exit_helper(void);
void			execute_helper(char **env, char **cmd, t_shell *shell);
char			*get_env_values_helper(char **env, const char *var);
int				handle_pipe_error(char last, char *line, int *i);
int				handle_output_redirection(char *line, int *i, char last);
int				handle_input_redirection(char *line, int *i, char last);
int				error_check_end_conditions(char last_char, int in_pipe,
					int in_redirection, int out_redirection);
int				handle_special_characters(t_special_char_data *data);
int				check_syntax(t_check_syntax_data *data);
int				error_check_on_pipe_and_redirection(char *line);
int				is_special_char(char c);
int				is_quoted(char *str, int index);
void			check_the_line(t_check_line_data *data);
char			*handle_operator(const char *s);
char			*handle_word(const char *s);

int				ft_malloc_check(char **arr, size_t i);
size_t			ft_lens(const char *c);
void			handle_quotes(const char *s, int *p);
int				check_triple_redirection(char *line);
size_t			ft_strnlen(const char *s, size_t maxlen);
int				operator_check(char str);
int				count_commands1(char **commands);
void			free_filtered_commands(char **filtered_commands, int j);
char			**filter_commands(char **commands, int cmd_count);
char			*handle_single_quotes(char *str, int *i);
char			*handle_dollar_sign(char *str, char **env, t_shell *shell,
					int *i);
char			*handle_regular_char(char *str, int *i);
char			*join_and_free(char *s1, char *s2);
// ... existing code ...

void			setup_echo_helper(t_cmds *cmds, int *original_stdout,
					int *original_stdin);
int				handle_echo_flags(t_cmds *cmds, int *i, int *newline);
void			print_echo_args(t_echo_data *data);

void			restore_echo_fd(int original_stdout, int original_stdin);
char			*create_new_var1(char *var, char *value);
void			update_existing_var1(char ***env, int var_index, char *new_var);
void			add_new_var1(char ***env, char *new_var);
// ... existing code ...
void			setup_and_print_echo(t_echo_helper_data *data);
// static int	handle_redirect(int *i, t_redirect_helper_data *helper_data);
int				execute_builtin_helper(t_cmds *cmds, char ***env,
					t_shell *shell);
int				is_valid_command(char *cmd, char **env);
int				count_commands_helper(t_cmds *cmds, char **env);
int				process_operators(const char *s, char **arr, int *i, int p);
void			handle_quotes_state(const char **str, t_quote_state *state);
const char		*handle_operator_count(const char *str, t_quote_state *state);
void			handle_child_process(t_cmds *cmds, char ***env, t_shell *shell,
					int i);
void			handle_heredoc_pipe(t_cmds *cmds, int i, int heredoc_pipe[2]);
void			handle_parent_process(int *prev_fd, int pipefd[2], int i,
					int cmds_size);
void			wait_for_children(t_cmds *cmds, t_shell *shell);
int				execution_helper(t_cmds *cmds, char ***env, t_shell *shell);
void			execution_loop(t_cmds *cmds, char ***env, t_shell *shell,
					int *prev_fd);
void			handle_fork(t_exec_params *params);
void			handle_fork_local(t_exec_params_local *params);

int				execute_builtin(t_cmds *cmds, char ***env, t_shell *shell);
void			execute(char **env, char **cmd, t_shell *shell);
int				execute_command(t_cmds *cmds, char ***env, t_shell *shell);

int				execution(t_cmds *cmds, char ***env, t_shell *shell);
int				parse_and_validate_exit_arg(char *arg, int *sign,
					long long *exit_code);
int				handle_exit(char **args);
int				is_valid_exit_arg(const char *arg);
long long		ft_atoll(const char *str);
char			*trim_quotes(char *str);
void			remove_env_var(char ***env, const char *var);
int				handle_redirections(char **cmd, t_cmds *cmds, char **env,
					t_shell *shell);
int				is_builtin_count(char *cmd);
int				count_env_size(char **env);
int				find_env_var(char **env, const char *var);
char			**allocate_new_env(int new_size);
void			copy_env(char **new_env, char **env);
char			**expand_env(char **env, int new_size);
void			construct_env_var(char *dest, const char *var,
					const char *value);
int				is_alphanumeric(const char *str);
void			print_envs(char **env);
void			extract_var_value(char *arg, char *var, char **value,
					int *append);
void			update_existing_var(t_update_var_params *params);
void			add_new_var(char ***env, const char *var, const char *value);
void			initialize_cmds(t_do_shit_data *data, t_cmds *cmds);
void			fill_cmd_args(t_do_shit_data *data, t_cmds *cmds);
void			handle_heredoc_wrapper(t_do_shit_data *data, t_cmds *cmd,
					int *i);
void			handle_redirect_wrapper(t_do_shit_data *data, t_cmds *cmd,
					int *i);
int				count_cmds(char **args);
int				count_cmd_args(char **args, int *i);
void			process_single_cmd(t_do_shit_data *data, t_cmds *cmd, int *i);
void			handle_heredoc_wrapper1(char *delimiter, t_cmds *cmd,
					char **env);
int				process_export_arg(char *arg, char ***env);
int				handle_export_var(char *var, char *value, int append,
					char ***env);
void			update_var(char ***env, int idx, const char *var,
					const char *value);
void			initialize_commands(t_do_shit_data *data, t_cmds **cmds_ptr);
void			allocate_command_args(t_do_shit_data *data, t_cmds *cmds);
void			process_arguments(t_do_shit_data *data, t_cmds *cmds);
char			*allocate_new_value4(const char *existing_value,
					const char *new_value, int append);
void			update_env_var(char ***env, int idx, const char *var,
					const char *new_value);
void			process_arguments(t_do_shit_data *data, t_cmds *cmds);
void			process_funny_argument1(t_do_shit_data *data, t_cmds *cmd,
					int *i, int *j);

#endif