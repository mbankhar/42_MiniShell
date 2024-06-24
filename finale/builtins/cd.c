// cd.c
#include "cd.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "../minishell.h"
#include "../env/new_env.h"

// Function to implement the built-in 'cd' command
int built_cd(Shell *shell, char **splitted, int splitted_len) {
    if (splitted_len > 2) {
        fprintf(stderr, "minishell: cd: too many arguments\n");
        return 1;
    }
    if (splitted_len == 1) {
        splitted = realloc(splitted, 2 * sizeof(char *));
        splitted[1] = strdup(getenv("HOME"));
        splitted_len = 2;
    }
    const char *root = splitted[1];
    set_env(&shell->env, "OLDPWD", get_env(&shell->env, "PWD"));
    if (chdir(root) == 0) {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            set_env(&shell->env, "PWD", cwd);
        } else {
            perror("getcwd");
            return 1;
        }
    } else {
        fprintf(stderr, "minishell: cd: %s: No such file or directory\n", splitted[1]);
        return 1;
    }
    return 0;
}
