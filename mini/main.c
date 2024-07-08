#include "minishell.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

void sigint_handler(int signal)
{
    if (signal == SIGINT)
    {
        printf("SIGINT received\n");
        printf("\n");
        fflush(stdout);
    }
}

void sigquit_handler(int signal)
{
    if (signal == SIGQUIT)
    {
        printf("Quit signal received, exiting.\n");
        exit(EXIT_FAILURE);
    }
}

void sigterm_handler(int signal)
{
    if (signal == SIGTERM)
    {
        printf("Termination signal received, exiting.\n");
        exit(EXIT_FAILURE);
    }
}

void look_for_redirect(char **commands, int index, t_cmds *cmds)
{
    if (commands[index][0] == '>' && commands[index][1] == '>')
        cmds->fd_out = open(commands[index + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (commands[index][0] == '<')
        cmds->fd_in = open(commands[index + 1], O_RDONLY);
    else if (commands[index][0] == '>')
        cmds->fd_out = open(commands[index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
}

void do_shit(char **args, t_exec *exec, t_cmds **cmds_ptr) {
    int i, j, k;
    t_cmds *cmds;

    i = -1;
    *cmds_ptr = malloc((exec->number_of_pipes + 1) * sizeof(t_cmds));
    if (*cmds_ptr == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    cmds = *cmds_ptr;
    k = 0;

    while (args[++i]) {
        j = 0;
        while (args[i] && args[i][0] != '|') {
            if (args[i][0] == '<' || args[i][0] == '>') {
                i = i + 2;
                continue;
            } else {
                i++;
                j++;
            }
        }
        cmds[k].cmd_args = malloc(sizeof(char *) * (j + 1));
        if (cmds[k].cmd_args == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        k++;
        if (!args[i]) break;
    }

    i = -1;
    k = 0;
    while (args[++i]) {
        j = 0;
        cmds[k].fd_in = -2;
        cmds[k].fd_out = -2;
        while (args[i] && args[i][0] != '|') {
            if (args[i][0] == '<' || args[i][0] == '>') {
                look_for_redirect(args, i, &cmds[k]);
                i = i + 2;
                continue;
            } else {
                cmds[k].cmd_args[j] = strdup(args[i]);
                if (cmds[k].cmd_args[j] == NULL) {
                    perror("strdup failed");
                    exit(EXIT_FAILURE);
                }
                j++;
                i++;
            }
        }
        cmds[k].cmd_args[j] = NULL;
        k++;
        if (!args[i]) break;
    }
    cmds->size = k;
}

void remove_quotes(char **args)
{
    int i, j, length, index;
    char *temp;

    if (args == NULL)
        return;

    i = -1;
    while (args[++i] != NULL)
    {
        length = strlen(args[i]);
        if (length > 1 && ((args[i][0] == '\'' && args[i][length - 1] == '\'') || (args[i][0] == '\"' && args[i][length - 1] == '\"')))
        {
            temp = malloc(sizeof(char) * (length - 1));
            if (temp == NULL)
            {
                perror("malloc failed");
                exit(EXIT_FAILURE);
            }
            j = 1;
            index = 0;
            while (j < length - 1)
            {
                temp[index] = args[i][j];
                index++;
                j++;
            }
            temp[index] = '\0';
            free(args[i]);
            args[i] = temp;
        }
    }
}

bool check_the_line(char *line, t_exec *exec, t_cmds *cmds)
{
    int count;
    bool should_continue = true;
    char **args;
    extern char **environ;

    count = 0;
    if (are_quotes_even(line) != 0)
    {
        exec->number_of_pipes = count_char_occurrences(line, '|');
        check_dollar(&line, exec);
        args = ft_splitspecial(line, '|');
        do_shit(args, exec, &cmds);  // Pass the address of cmds
        exec->tokens = get_the_token(args, exec);

        if (cmds->size > 0 && is_builtin_command(cmds->cmd_args[0]))

        {
            execute_builtin(cmds, environ);
        }
        else
        {
            execution(cmds, environ, exec);
        }

        if (cmds->size > 0 && strcmp(cmds->cmd_args[0], "exit") == 0)

            should_continue = false;

        ft_free(args);
    }
    else
        printf("Quotes error\n");

    return should_continue;
}

int main(int argc, char **argv, char **envp) {
    char *line;
    t_exec exec;
    t_cmds *cmds = NULL;  // Move this declaration outside the loop
    struct sigaction sa;

    (void)argc;
    (void)argv;
    (void)envp;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigint_handler;
    sigaction(SIGINT, &sa, NULL);

    // Set default PATH if not set
    if (getenv("PATH") == NULL) {
        setenv("PATH", "/usr/bin:/bin", 1);
    }

    while (1) {
        line = readline("minishell> ");
        if (!line) break;
        if (strlen(line) > 0) {
            add_history(line);
            if (!check_the_line(line, &exec, cmds)) break;
        }
        free(line);

        // Free cmds here if it's not null
        if (cmds != NULL) {
            // Free each cmd_args
            for (int i = 0; i < cmds->size; i++) {
                if (cmds[i].cmd_args != NULL) {
                    // Free each command argument
                    for (int j = 0; cmds[i].cmd_args[j] != NULL; j++) {
                        free(cmds[i].cmd_args[j]);
                    }
                    free(cmds[i].cmd_args);
                }
            }
            free(cmds);
            cmds = NULL;
        }
    }
    return 0;
}
