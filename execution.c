// execution.c
#include "minishell.h"

void execute_command(Command *cmd) {
    if (cmd->tokens[0] == NULL) {
        return; // No command to execute
    }

    if (is_builtin(cmd->tokens[0])) {
        execute_builtin(cmd);
        return;
    }

    pid_t pid = fork();
    if (pid == 0) { // Child process
        if (cmd->input_fd != 0) {
            dup2(cmd->input_fd, 0);
            close(cmd->input_fd);
        }
        if (cmd->output_fd != 1) {
            dup2(cmd->output_fd, 1);
            close(cmd->output_fd);
        }

        execvp(cmd->tokens[0], cmd->tokens);
        perror("minishell: execvp");
        exit(1);
    } else if (pid > 0) { // Parent process
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("minishell: fork");
        exit(1);
    }
}

int is_builtin(char *command) {
    return (strcmp(command, "cd") == 0 || strcmp(command, "echo") == 0 ||
            strcmp(command, "export") == 0 || strcmp(command, "unset") == 0 ||
            strcmp(command, "env") == 0 || strcmp(command, "exit") == 0);
}

void execute_builtin(Command *cmd) {
    if (strcmp(cmd->tokens[0], "cd") == 0) {
        if (cmd->tokens[1] == NULL) {
            fprintf(stderr, "minishell: cd: missing argument\n");
        } else {
            change_directory(cmd->tokens[1]);
        }
    } else if (strcmp(cmd->tokens[0], "echo") == 0) {
        execute_echo(cmd->tokens);
    } else if (strcmp(cmd->tokens[0], "export") == 0) {
        if (cmd->tokens[1] == NULL) {
            fprintf(stderr, "minishell: export: missing argument\n");
        } else {
            char *var = strtok(cmd->tokens[1], "=");
            char *value = strtok(NULL, "=");
            execute_export(var, value);
        }
    } else if (strcmp(cmd->tokens[0], "unset") == 0) {
        if (cmd->tokens[1] == NULL) {
            fprintf(stderr, "minishell: unset: missing argument\n");
        } else {
            execute_unset(cmd->tokens[1]);
        }
    } else if (strcmp(cmd->tokens[0], "env") == 0) {
        print_env();
    } else if (strcmp(cmd->tokens[0], "exit") == 0) {
        exit(0);
    }
}

void change_directory(char *path) {
    if (chdir(path) == -1) {
        perror("minishell: chdir");
    }
}

void execute_echo(char *args[]) {
    for (int i = 1; args[i] != NULL; i++) {
        printf("%s", args[i]);
        if (args[i + 1] != NULL) {
            printf(" ");
        }
    }
    printf("\n");
}

void execute_export(char *var, char *value) {
    if (setenv(var, value, 1) == -1) {
        perror("minishell: setenv");
    }
}

void execute_unset(char *var) {
    if (unsetenv(var) == -1) {
        perror("minishell: unsetenv");
    }
}

void print_env() {
    extern char **environ;
    for (char **env = environ; *env != 0; env++) {
        printf("%s\n", *env);
    }
}
