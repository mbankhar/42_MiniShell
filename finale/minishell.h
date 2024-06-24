// minishell.h
#ifndef MINISHELL_H
#define MINISHELL_H

#include "env/new_env.h"
#include "parser/commands.h"

struct ParsedHead;  // Forward declaration
struct Pipe;        // Forward declaration

typedef struct Shell {
    char *line;
    Env env;
    struct ParsedHead *tokens;
    int error;
    int last_error;
} Shell;

void init_shell(Shell *shell);
void reset_shell(Shell *shell);
void display_prompt(const Shell *shell);
void minishell();

#endif // MINISHELL_H
