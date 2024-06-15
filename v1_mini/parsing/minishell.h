#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
// # include "../libft/libft.h"

// Define token types
typedef enum {
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_SEMICOLON,
    TOKEN_REDIRECT_LEFT,
    TOKEN_REDIRECT_RIGHT,
    TOKEN_DOUBLE_REDIRECT_LEFT,
    TOKEN_DOUBLE_REDIRECT_RIGHT,
    TOKEN_PIPELINE,
    TOKEN_COMMAND,
    TOKEN_OPTION,
    TOKEN_SINGLE_QUOTE,
    TOKEN_DOUBLE_QUOTE,
    TOKEN_BACKSLASH,
    TOKEN_LITERAL,
    TOKEN_VARIABLE,
    TOKEN_SPACE,
    TOKEN_ERROR
} TokenType;

// Define a struct for Token
typedef struct s_token {
    TokenType type;
    char *value;
    struct s_token *next;
} Token;

// Define a struct for Builtin commands
typedef struct s_builtin {
    char *name;
    int (*func)(char **args);
} Builtin;

// Define a struct for Operator
typedef struct s_operator {
    TokenType type;
    int precedence;
} Operator;

// Define a struct for Shell info
typedef struct s_shell_info {
    Token *tokens;
    char **envp;
} ShellInfo;

// Function prototypes
Token *tokenize_input(char *input);
void free_tokens(Token *tokens);
void execute_commands(ShellInfo *info);
char *get_executable_path(char **env, char *cmd);

void add_token(Token **tokens, TokenType type, const char *value);
void parse_args(char **input, Token **tokens);
void parse_pipe(char **input, Token **tokens);
void parse_sep(char **input, Token **tokens);
void parse_redir(char **input, Token **tokens);
void parse_options(char *input, Token **tokens);
void parse_d_quotes(char **input, Token **tokens);
void parse_varenv(char **input, Token **tokens);
const char *token_type_to_string(TokenType type);


#endif
