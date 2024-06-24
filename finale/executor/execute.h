// execute.h
#ifndef EXECUTE_H
#define EXECUTE_H

#include "../minishell.h"

struct Shell;  // Forward declaration
struct Pipe;   // Forward declaration

typedef struct Pipe {
    int pipe_in;
    int pipe_out;
    char *value;
} Pipe;

Pipe new_pipe();
int open_read(Pipe *pipe, const char *path);
int open_write(Pipe *pipe, const char *path);
int verify_open(int file, const char *value, int *pipe);
struct Pipe execute_pipes(struct Shell *shell, ElementLine *tokens, int token_count, int now, int *error, struct Pipe pipe);
struct Shell execute_command(struct Shell shell);
struct Pipe execute(ElementLine *element, struct Shell *shell, struct Pipe pipe, int last); // Declare execute here

#endif // EXECUTE_H
