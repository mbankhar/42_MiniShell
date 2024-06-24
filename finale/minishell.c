// minishell.c
#include "minishell.h"
#include "executor/execute.h"
#include "parser/lexer.h"
#include "parser/commands.h"
#include "env/new_env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

void init_shell(Shell *shell) {
    shell->line = NULL;
    shell->env = new_env();
    shell->tokens = new_parsed_head();
    shell->error = 0;
    shell->last_error = 0;
}

void reset_shell(Shell *shell) {
    if (shell->line != NULL) {
        free(shell->line);
        shell->line = NULL;
    }
    shell->last_error = shell->error;
    free(shell->tokens->tokens);
    free(shell->tokens);
    shell->tokens = new_parsed_head();
    shell->error = 0;
}

void display_prompt(const Shell *shell) {
    const char *user = get_env((Env *)&shell->env, "USER");
    if (user != NULL) {
        printf("%s:", user);
    } else {
        char hostname[1024];
        if (gethostname(hostname, sizeof(hostname)) == 0) {
            printf("%s:", hostname);
        } else {
            perror("gethostname");
        }
    }
    const char *pwd = get_env((Env *)&shell->env, "PWD");
    if (pwd != NULL) {
        const char *dir = strrchr(pwd, '/');
        printf("%s$ ", dir ? dir + 1 : pwd);
    } else {
        printf("$ ");
    }
    fflush(stdout);
}

void minishell() {
    Shell shell;
    init_shell(&shell);

    while (1) {
        display_prompt(&shell);
        size_t len = 0;
        if (getline(&shell.line, &len, stdin) == -1) {
            perror("getline");
            exit(EXIT_FAILURE);
        }
        shell.tokens = parser(shell.line, &shell.error);
        if (shell.error != 0) {
            reset_shell(&shell);
            continue;
        }
        shell = execute_command(shell);
        reset_shell(&shell);
    }
}

int main() {
    minishell();
    return 0;
}
