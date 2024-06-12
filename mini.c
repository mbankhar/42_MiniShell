// // // #include <stdio.h>
// // // #include <fcntl.h> // For file operations
// // // #include <readline/history.h>
// // // #include <readline/readline.h>
// // // #include <stdlib.h>
// // // #include <string.h>
// // // #include <unistd.h> // For dup2

// // // #define MAX_TOKENS 128 // Adjust as needed

// // // // Function to tokenize the input line
// // // char	**tokenize_input(char *line)
// // // {
// // // 	char	**tokens;
// // // 	int		i;
// // // 	char	*start;

// // // 	tokens = malloc(MAX_TOKENS * sizeof(char *));
// // // 	i = 0;
// // // 	int in_quotes = 0; // 0: no quotes, 1: single quotes, 2: double quotes
// // // 	start = line;
// // // 	for (char *c = line; *c != '\0'; c++)
// // // 	{
// // // 		if (*c == '\'' || *c == '"')
// // // 		{
// // // 			if (in_quotes == 0)
// // // 			{
// // // 				in_quotes = (*c == '\'') ? 1 : 2;
// // // 				start = c + 1;
// // // 			}
// // // 			else if (in_quotes == 1 && *c == '\'' || in_quotes == 2
// // // 				&& *c == '"')
// // // 			{ // Corrected condition
// // // 				in_quotes = 0;
// // // 				*c = '\0';
// // // 				tokens[i++] = start;
// // // 				start = c + 1;
// // // 			}
// // // 		}
// // // 		else if (*c == ' ' && in_quotes == 0)
// // // 		{
// // // 			*c = '\0';
// // // 			if (start != c)
// // // 			{ // Avoid empty tokens
// // // 				tokens[i++] = start;
// // // 			}
// // // 			start = c + 1;
// // // 		}
// // // 	}
// // // 	if (start != line + strlen(line))
// // // 	{ // Add the last token
// // // 		tokens[i++] = start;
// // // 	}
// // // 	tokens[i] = NULL;
// // // 	return (tokens);
// // // }
// // // int	main(void)
// // // {
// // // 	char	*line;
// // // 	char	**tokens;
// // // 	int		output_fd;
// // // 	int		flags;

// // // 	while (1)
// // // 	{
// // // 		line = readline("maties> ");
// // // 		if (!line)
// // // 		{
// // // 			break ; // Handle Ctrl+D (EOF)
// // // 		}
// // // 		add_history(line);
// // // 		// Tokenization
// // // 		tokens = tokenize_input(line);
// // // 		int input_fd = 0; // Standard input (stdin)
// // // 		output_fd = 1;
// // // 		int pipe_fd[2]; // For pipe communication
// // // 		for (int j = 0; tokens[j] != NULL; j++)
// // // 		{
// // // 			if (strcmp(tokens[j], "|") == 0)
// // // 			{
// // // 				if (pipe(pipe_fd) == -1)
// // // 				{
// // // 					perror("minishell: pipe");
// // // 					exit(1);
// // // 				}
// // // 				output_fd = pipe_fd[1];
// // // 			}
// // // 			for (int j = 0; tokens[j] != NULL; j++)
// // // 			{
// // // 				if (strcmp(tokens[j], "<") == 0)
// // // 				{ // Input redirection
// // // 					if (tokens[j + 1] != NULL)
// // // 					{
// // // 						input_fd = open(tokens[j + 1], O_RDONLY);
// // // 							// Open in read-only mode
// // // 						if (input_fd == -1)
// // // 						{
// // // 							perror("minishell: open");
// // // 							exit(1);
// // // 						}
// // // 						j++; // Skip the filename
// // // 					}
// // // 					else
// // // 					{
// // // 						fprintf(stderr,
// // // 							"minishell: syntax error: filename expected after '<'\n");
// // // 						exit(1);
// // // 					}
// // // 				}
// // // 				else if (strcmp(tokens[j], ">") == 0 || strcmp(tokens[j],
// // // 						">>") == 0)
// // // 				{ // Output redirection
// // // 					flags = O_WRONLY | O_CREAT;
// // // 					if (strcmp(tokens[j], ">>") == 0)
// // // 					{
// // // 						flags |= O_APPEND;
// // // 					}
// // // 					if (tokens[j + 1] != NULL)
// // // 					{
// // // 						output_fd = open(tokens[j + 1], flags, 0644);
// // // 						if (output_fd == -1)
// // // 						{
// // // 							perror("minishell: open");
// // // 							exit(1);
// // // 						}
// // // 						j++; // Skip the filename
// // // 					}
// // // 					else
// // // 					{
// // // 						fprintf(stderr,
// // // 							"minishell: syntax error: filename expected after '%s'\n",
// // // 							tokens[j]);
// // // 						exit(1);
// // // 					}
// // // 				}

// // // 				if (tokens[j][0] == '$') {
// // //                 char *var_name = tokens[j] + 1;
// // //                 char *var_value = getenv(var_name);
// // //                 if (var_value != NULL) {
// // //                     tokens[j] = var_value;
// // //                 } else {
// // //                     // Handle the case where the variable is not found
// // //                 }
// // //             }
// // // 			}
// // // 			// ... (Before executing the command)
// // // 			 pid_t pid = fork();
// // //         	if (pid == 0) { // Child process
// // //             // ... (Before executing the command)
// // //             if (input_fd != 0) {
// // //                 dup2(input_fd, 0);
// // //                 close(input_fd);
// // //             }
// // //             if (output_fd != 1) {
// // //                 dup2(output_fd, 1);
// // //                 close(output_fd);
// // //             }

// // //             if (execvp(tokens[0], tokens) == -1) {
// // //                 perror("minishell: execvp");
// // //                 exit(1);
// // //             }
// // //         } else if (pid > 0) { // Parent process
// // //             // ... (After executing the command)
// // //             if (output_fd == pipe_fd[1]) {
// // //                 close(pipe_fd[1]);
// // //                 input_fd = pipe_fd[0];
// // //             } else {
// // //                 input_fd = 0;
// // //             }
// // //             output_fd = 1;

// // //             wait(NULL);
// // //         } else {
// // //             perror("minishell: fork");
// // //             exit(1);
// // //         }

// // //         // Free memory
// // //         free(tokens);
// // //         free(line);
// // //     }

// // //     return 0;
// // // }
// // // }



// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <string.h>
// // #include <fcntl.h>
// // #include <unistd.h>
// // #include <readline/readline.h>
// // #include <readline/history.h>
// // #include <sys/wait.h>

// // #define MAX_TOKENS 128

// // // Function to tokenize the input line (with enhanced features)
// // char **tokenize_input(char *line) {
// //     char **tokens = malloc(MAX_TOKENS * sizeof(char *));
// //     int i = 0;
// //     int in_quotes = 0;
// //     char *start = line;
// //     int escape_next = 0; // Flag to indicate if the next character is escaped

// //     for (char *c = line; *c != '\0'; c++) {
// //         if (escape_next) {
// //             escape_next = 0; // Reset the escape flag
// //             continue; // Skip the escaped character
// //         }

// //         if (*c == '\\') {
// //             escape_next = 1; // Set the escape flag for the next character
// //             continue;
// //         }

// //         if (*c == '\'' || *c == '"') {
// //             if (in_quotes == 0) {
// //                 in_quotes = (*c == '\'') ? 1 : 2;
// //                 start = c + 1;
// //             } else if ((in_quotes == 1 && *c == '\'') || (in_quotes == 2 && *c == '"')) {
// //                 in_quotes = 0;
// //                 *c = '\0';
// //                 tokens[i++] = start;
// //                 start = c + 1;
// //             }
// //         } else if (*c == ' ' && in_quotes == 0) {
// //             *c = '\0';
// //             if (start != c) { // Avoid empty tokens
// //                 tokens[i++] = start;
// //             }
// //             start = c + 1;
// //         } else if (*c == ';' && in_quotes == 0) {
// //             *c = '\0'; // Replace semicolon with null terminator
// //             if (start != c) {
// //                 tokens[i++] = start;
// //             }
// //             start = c + 1; // Start of the next command
// //         }
// //     }

// //     if (start != line + strlen(line)) { // Add the last token
// //         tokens[i++] = start;
// //     }
// //     tokens[i] = NULL;
// //     return tokens;
// // }

// // // Function to check if a command is a built-in command
// // int is_builtin(char *command) {
// //     // Example built-in commands
// //     if (strcmp(command, "cd") == 0 || strcmp(command, "exit") == 0) {
// //         return 1;
// //     }
// //     return 0;
// // }

// // // Function to execute built-in commands
// // void execute_builtin(char **tokens) {
// //     if (strcmp(tokens[0], "cd") == 0) {
// //         if (tokens[1] == NULL) {
// //             fprintf(stderr, "minishell: cd: missing argument\n");
// //         } else {
// //             if (chdir(tokens[1]) != 0) {
// //                 perror("minishell: cd");
// //             }
// //         }
// //     } else if (strcmp(tokens[0], "exit") == 0) {
// //         exit(0);
// //     }
// // }

// // int main(void) {
// //     char *line;
// //     char **tokens;

// //     while (1) {
// //         line = readline("maties> ");
// //         if (!line) {
// //             break; // Handle Ctrl+D (EOF)
// //         }
// //         add_history(line);

// //         tokens = tokenize_input(line);
// //         int input_fd = 0; // Standard input (stdin)
// //         int output_fd = 1; // Standard output (stdout)
// //         int pipe_fd[2]; // For pipe communication

// //         for (int j = 0; tokens[j] != NULL; j++) {
// //             if (strcmp(tokens[j], "|") == 0) {
// //                 if (pipe(pipe_fd) == -1) {
// //                     perror("minishell: pipe");
// //                     exit(1);
// //                 }
// //                 output_fd = pipe_fd[1];
// //             }

// //             if (strcmp(tokens[j], "<") == 0) { // Input redirection
// //                 if (tokens[j + 1] != NULL) {
// //                     input_fd = open(tokens[j + 1], O_RDONLY);
// //                     if (input_fd == -1) {
// //                         perror("minishell: open");
// //                         exit(1);
// //                     }
// //                     j++; // Skip the filename
// //                 } else {
// //                     fprintf(stderr, "minishell: syntax error: filename expected after '<'\n");
// //                     exit(1);
// //                 }
// //             } else if (strcmp(tokens[j], ">") == 0 || strcmp(tokens[j], ">>") == 0) { // Output redirection
// //                 int flags = O_WRONLY | O_CREAT;
// //                 if (strcmp(tokens[j], ">>") == 0) {
// //                     flags |= O_APPEND;
// //                 }
// //                 if (tokens[j + 1] != NULL) {
// //                     output_fd = open(tokens[j + 1], flags, 0644);
// //                     if (output_fd == -1) {
// //                         perror("minishell: open");
// //                         exit(1);
// //                     }
// //                     j++; // Skip the filename
// //                 } else {
// //                     fprintf(stderr, "minishell: syntax error: filename expected after '%s'\n", tokens[j]);
// //                     exit(1);
// //                 }
// //             }

// //             if (tokens[j][0] == '$') { // Environment variable expansion
// //                 char *var_name = tokens[j] + 1;
// //                 char *var_value = getenv(var_name);
// //                 if (var_value != NULL) {
// //                     tokens[j] = var_value;
// //                 }
// //             }
// //         }

// //         pid_t pid = fork();
// //         if (pid == 0) { // Child process
// //             if (is_builtin(tokens[0])) {
// //                 execute_builtin(tokens);
// //             } else {
// //                 if (input_fd != 0) {
// //                     dup2(input_fd, 0);
// //                     close(input_fd);
// //                 }
// //                 if (output_fd != 1) {
// //                     dup2(output_fd, 1);
// //                     close(output_fd);
// //                 }

// //                 execvp(tokens[0], tokens);
// //                 perror("minishell: execvp");
// //                 exit(1);
// //             }
// //         } else if (pid > 0) { // Parent process
// //             if (output_fd == pipe_fd[1]) {
// //                 close(pipe_fd[1]);
// //                 input_fd = pipe_fd[0];
// //             } else {
// //                 input_fd = 0;
// //             }
// //             output_fd = 1;

// //             wait(NULL);
// //         } else {
// //             perror("minishell: fork");
// //             exit(1);
// //         }

// //         // Free memory
// //         free(tokens);
// //         free(line);
// //     }

// //     return 0;
// // }


// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <readline/readline.h>
// #include <readline/history.h>

// #define MAX_TOKENS 128

// typedef struct Command {
//     char **tokens; // Array of command tokens
//     int input_fd;  // Input file descriptor
//     int output_fd; // Output file descriptor
// } Command;

// // Function prototypes
// Command *parse_input(char *line);
// void free_command(Command *cmd);

// // Function to parse the input line into commands
// Command *parse_input(char *line) {
//     Command *cmd = malloc(sizeof(Command));
//     cmd->tokens = malloc(MAX_TOKENS * sizeof(char *));
//     cmd->input_fd = 0; // Default to stdin
//     cmd->output_fd = 1; // Default to stdout

//     int i = 0;
//     int in_quotes = 0;
//     char *start = line;
//     int escape_next = 0; // Flag to indicate if the next character is escaped

//     for (char *c = line; *c != '\0'; c++) {
//         if (escape_next) {
//             escape_next = 0; // Reset the escape flag
//             continue; // Skip the escaped character
//         }

//         if (*c == '\\') {
//             escape_next = 1; // Set the escape flag for the next character
//             continue;
//         }

//         if (*c == '\'' || *c == '"') {
//             if (in_quotes == 0) {
//                 in_quotes = (*c == '\'') ? 1 : 2;
//                 start = c + 1;
//             } else if ((in_quotes == 1 && *c == '\'') || (in_quotes == 2 && *c == '"')) {
//                 in_quotes = 0;
//                 *c = '\0';
//                 cmd->tokens[i++] = start;
//                 start = c + 1;
//             }
//         } else if (*c == ' ' && in_quotes == 0) {
//             *c = '\0';
//             if (start != c) { // Avoid empty tokens
//                 cmd->tokens[i++] = start;
//             }
//             start = c + 1;
//         } else if (*c == '<' && in_quotes == 0) { // Input redirection
//             *c = '\0';
//             if (start != c) {
//                 cmd->tokens[i++] = start;
//             }
//             start = c + 1;
//             if (*start != '\0' && *start != ' ') {
//                 cmd->input_fd = open(start, O_RDONLY);
//                 if (cmd->input_fd == -1) {
//                     perror("minishell: open");
//                     exit(1);
//                 }
//             } else {
//                 fprintf(stderr, "minishell: syntax error: filename expected after '<'\n");
//                 exit(1);
//             }
//         } else if ((*c == '>' || *c == '>') && in_quotes == 0) { // Output redirection
//             *c = '\0';
//             if (start != c) {
//                 cmd->tokens[i++] = start;
//             }
//             start = c + 1;
//             int flags = O_WRONLY | O_CREAT;
//             if (*start == '>') { // Append mode
//                 flags |= O_APPEND;
//                 start++;
//             }
//             if (*start != '\0' && *start != ' ') {
//                 cmd->output_fd = open(start, flags, 0644);
//                 if (cmd->output_fd == -1) {
//                     perror("minishell: open");
//                     exit(1);
//                 }
//             } else {
//                 fprintf(stderr, "minishell: syntax error: filename expected after '>'\n");
//                 exit(1);
//             }
//         }
//     }

//     if (start != line + strlen(line)) { // Add the last token
//         cmd->tokens[i++] = start;
//     }
//     cmd->tokens[i] = NULL;

//     // Environment variable expansion
//     for (int j = 0; cmd->tokens[j] != NULL; j++) {
//         if (cmd->tokens[j][0] == '$') {
//             char *var_name = cmd->tokens[j] + 1;
//             char *var_value = getenv(var_name);
//             if (var_value != NULL) {
//                 cmd->tokens[j] = var_value;
//             }
//         }
//     }

//     return cmd;
// }

// // Function to free the command structure
// void free_command(Command *cmd) {
//     free(cmd->tokens);
//     free(cmd);
// }


// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/wait.h>
// #include <fcntl.h>

// // Struct definition
// typedef struct Command {
//     char **tokens; // Array of command tokens
//     int input_fd;  // Input file descriptor
//     int output_fd; // Output file descriptor
// } Command;

// // Function prototypes
// void execute_command(Command *cmd);
// int is_builtin(char *command);
// void execute_builtin(Command *cmd);

// // Function to execute commands
// void execute_command(Command *cmd) {
//     if (cmd->tokens[0] == NULL) {
//         return; // No command to execute
//     }

//     if (is_builtin(cmd->tokens[0])) {
//         execute_builtin(cmd);
//         return;
//     }

//     pid_t pid = fork();
//     if (pid == 0) { // Child process
//         if (cmd->input_fd != 0) {
//             dup2(cmd->input_fd, 0);
//             close(cmd->input_fd);
//         }
//         if (cmd->output_fd != 1) {
//             dup2(cmd->output_fd, 1);
//             close(cmd->output_fd);
//         }

//         execvp(cmd->tokens[0], cmd->tokens);
//         perror("minishell: execvp");
//         exit(1);
//     } else if (pid > 0) { // Parent process
//         wait(NULL);
//     } else {
//         perror("minishell: fork");
//         exit(1);
//     }
// }

// // Function to check if a command is a built-in command
// int is_builtin(char *command) {
//     if (strcmp(command, "cd") == 0 || strcmp(command, "exit") == 0) {
//         return 1;
//     }
//     return 0;
// }

// // Function to execute built-in commands
// void execute_builtin(Command *cmd) {
//     if (strcmp(cmd->tokens[0], "cd") == 0) {
//         if (cmd->tokens[1] == NULL) {
//             fprintf(stderr, "minishell: cd: missing argument\n");
//         } else {
//             if (chdir(cmd->tokens[1]) != 0) {
//                 perror("minishell: cd");
//             }
//         }
//     } else if (strcmp(cmd->tokens[0], "exit") == 0) {
//         exit(0);
//     }
// }
