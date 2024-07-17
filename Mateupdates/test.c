#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MAX_DELIMITER_LENGTH 256

char *get_temp_filename()
{
    static int count = 0;
    char *filename = malloc(20);
    if (!filename)
        return NULL;
    snprintf(filename, 20, "/tmp/heredoc_%d", count++);
    return filename;
}

int handle_heredoc(const char *delimiter)
{
    char *line;
    char *temp_file = get_temp_filename();
    int fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    
    if (fd == -1)
    {
        perror("Error opening temporary file");
        free(temp_file);
        return -1;
    }

    while (1)
    {
        line = readline("> ");
        if (!line)
            break;

        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }

        write(fd, line, strlen(line));
        write(fd, "\n", 1);
        free(line);
    }

    close(fd);
    return open(temp_file, O_RDONLY);
}

void execute_command(char *command, int input_fd)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        // Child process
        if (input_fd != STDIN_FILENO)
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        char *args[] = {"/bin/sh", "-c", command, NULL};
        execve("/bin/sh", args, NULL);
        perror("execve");
        exit(1);
    }
    else if (pid > 0)
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
    else
    {
        perror("fork");
    }
}

int main()
{
    char *input;
    char delimiter[MAX_DELIMITER_LENGTH];

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break;

        add_history(input);

        if (strncmp(input, "<<", 2) == 0)
        {
            char *delim_start = input + 2;
            while (*delim_start == ' ') delim_start++;
            
            if (*delim_start == '\0')
            {
                printf("Error: No delimiter specified for heredoc\n");
                free(input);
                continue;
            }

            char *delim_end = delim_start;
            while (*delim_end && !isspace(*delim_end)) delim_end++;
            
            size_t delim_len = delim_end - delim_start;
            if (delim_len >= MAX_DELIMITER_LENGTH)
            {
                printf("Error: Delimiter too long\n");
                free(input);
                continue;
            }

            strncpy(delimiter, delim_start, delim_len);
            delimiter[delim_len] = '\0';

            printf("Enter heredoc content. End with '%s' on a new line.\n", delimiter);
            int heredoc_fd = handle_heredoc(delimiter);
            if (heredoc_fd != -1)
            {
                printf("Heredoc content:\n");
                execute_command("cat", heredoc_fd);
                close(heredoc_fd);
            }
        }
        else if (strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }
        else if (strlen(input) > 0)
        {
            execute_command(input, STDIN_FILENO);
        }

        free(input);
    }

    rl_clear_history();
    return 0;
}





