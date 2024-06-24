// execute.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "execute.h"
#include "../minishell.h"
#include "../parser/commands.h"
#include "../redirections/heredoc.h"

// Function prototypes
Pipe new_pipe();
int open_read(Pipe *pipe, const char *path);
int open_write(Pipe *pipe, const char *path);
int verify_open(int file, const char *value, int *pipe);
Pipe execute_pipes(Shell *shell, ElementLine *tokens, int token_count, int now, int *error, Pipe pipe);
Shell execute_command(Shell shell);

// Initialize a new Pipe
Pipe new_pipe() {
    Pipe pipe;
    pipe.pipe_in = STDIN_FILENO;
    pipe.pipe_out = STDOUT_FILENO;
    pipe.value = NULL;
    return pipe;
}

// Open a file for reading and associate it with the pipe
int open_read(Pipe *pipe, const char *path) {
    int file = open(path, O_RDONLY);
    return verify_open(file, path, &pipe->pipe_in);
}

// Open a file for writing and associate it with the pipe
int open_write(Pipe *pipe, const char *path) {
    int file = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    return verify_open(file, path, &pipe->pipe_out);
}

// Verify if a file is opened successfully
int verify_open(int file, const char *value, int *pipe) {
    if (file >= 0) {
        *pipe = file;
        return 0;
    } else {
        fprintf(stderr, "minishell: no such file or directory: %s\n", value);
        return 1;
    }
}

// Execute pipes based on tokens
Pipe execute_pipes(Shell *shell, ElementLine *tokens, int token_count, int now, int *error, Pipe pipe) {
    if (now < token_count) {
        ElementLine token_now = tokens[now];
        if (token_now.parse_type == WORD) {
            pipe = execute_pipes(shell, tokens, token_count, now + 1, error, pipe);
            if (!(*error)) {
                pipe = execute(&token_now, shell, pipe, now == token_count - 1);
            }
            return pipe;
        } else if (token_now.parse_type == REDIRECTION) {
            char *fi = strdup(tokens[now + 1].value);
            if (!fi) {
                perror("strdup");
                *error = 1;
                return pipe;
            }
            if (strcmp(token_now.value, ">") == 0) {
                *error = open_write(&pipe, fi);
            } else if (strcmp(token_now.value, "<") == 0) {
                *error = open_read(&pipe, fi);
            } else if (strcmp(token_now.value, ">>") == 0) {
                *error = open_write(&pipe, fi);
            } else if (strcmp(token_now.value, "<<") == 0) {
                heredoc(fi);
            }
            free(fi);
            pipe = execute_pipes(shell, tokens, token_count, now + 2, error, pipe);
        }
    }
    return pipe;
}

// Execute the command
Shell execute_command(Shell shell) {
    if (shell.tokens == NULL) {
        fprintf(stderr, "Error: shell.tokens is NULL\n");
        exit(EXIT_FAILURE);
    }

    int cmds = 0;
    int last = 0;
    int error = 0;
    Pipe pipe = new_pipe();

    while (cmds < shell.tokens->n_cmds) {
        if (cmds + 1 == shell.tokens->n_cmds) {
            pipe.pipe_out = STDOUT_FILENO;
        } else {
            pipe.pipe_out = -1; // Replace with appropriate pipe creation
        }

        ElementLine *tokens = &shell.tokens->tokens[last];
        int token_count = shell.tokens->n_cmds - last;
        
        pipe = execute_pipes(&shell, tokens, token_count, 0, &error, pipe);
        if (error) {
            fprintf(stderr, "Error executing command\n");
            break;
        }
        last += token_count;
        cmds++;
    }

    return shell;
}

// Execute a single command
struct Pipe execute(ElementLine *element, Shell *shell, struct Pipe pipe, int last) {
    if (!element || !element->value) {
        fprintf(stderr, "Invalid element or element->value is NULL in execute\n");
        exit(EXIT_FAILURE);
    }

    char **splitted = split_string(element, shell);
    if (is_builtin(shell, splitted[0], splitted) == 1) {
        if (last) {
            pipe.pipe_in = -1;
            return pipe;
        }
        pipe.pipe_in = pipe.pipe_out;
        pipe.pipe_out = -1;
        return pipe;
    }
    if (path_validation(&shell->env, splitted[0])) {
        pid_t pid = fork();
        if (pid == 0) {
            dup2(pipe.pipe_in, STDIN_FILENO);
            dup2(pipe.pipe_out, STDOUT_FILENO);
            if (execvp(splitted[0], splitted) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else if (pid < 0) {
            perror("fork");
        } else {
            int status;
            waitpid(pid, &status, 0);
            shell->last_error = WEXITSTATUS(status);
            if (last) {
                pipe.pipe_in = -1;
                pipe.pipe_out = -1;
                return pipe;
            }
            pipe.pipe_in = -1; // Replace with actual pipe output
            pipe.pipe_out = -1;
        }
    } else {
        fprintf(stderr, "minishell: %s: command not found\n", splitted[0]);
        shell->error = 127;
        pipe.pipe_in = pipe.pipe_out;
        pipe.pipe_out = -1;
    }
    return pipe;
}
