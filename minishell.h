// minishell.h
#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_TOKENS 128

// Structure to hold parsed command details
typedef struct Command {
    char **tokens; // Array of command tokens
    int input_fd;  // Input file descriptor
    int output_fd; // Output file descriptor
} Command;

// Function prototypes for parsing
Command *parse_input(char *line);
void free_command(Command *cmd);

// Function prototypes for execution
void execute_command(Command *cmd);
void execute_builtin(Command *cmd);
int is_builtin(char *command);
void change_directory(char *path);
void execute_echo(char *args[]);
void execute_export(char *var, char *value);
void execute_unset(char *var);
void print_env();

#endif // MINISHELL_H
