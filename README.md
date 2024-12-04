# Minishell

A lightweight shell implementation built in C, inspired by Bash. This shell supports basic shell functionalities such as built-in commands, redirections, piping, and more.

## Features

- **Built-in Commands**: Includes `cd`, `echo`, `exit`, and more.
- **Redirections**: Input and output redirections (`>`, `<`, `>>`).
- **Pipelines**: Support for chaining commands with `|`.
- **Environment Variables**: Expands variables like `$PATH`, `$HOME`.
- **Heredoc**: Handles `<<` for input redirection.
- **Signal Handling**: Interactive signal handling for `Ctrl+C`, `Ctrl+Z`, etc.
- **Custom Parsing**: Supports parsing commands with quotes and special operators.

## Requirements

To build and run the project, you need:

- **C Compiler** (e.g., `gcc`)
- Libraries: `readline`, `termcap`, and `ncurses`

## Installation and Setup

1. Clone the repository:

   ```bash
   git clone <repository-url>
   cd <repository-directory>
   ```

2. Ensure the required libraries are installed. For most Unix-based systems, use:

   ```bash
   sudo apt-get install libreadline-dev libncurses-dev
   ```

3. Modify the `Makefile` if needed to adapt paths for your environment.

## Compilation

To compile the project, run:

```bash
make
```

This will produce an executable named `minishell`.

## Running the Shell

Start the shell by running:

```bash
./minishell
```

You’ll see the prompt:

```text
minishell>
```

## Built-in Commands

- **`cd [directory]`**: Change the current working directory.
- **`echo [text]`**: Print text to standard output.
- **`exit [status]`**: Exit the shell with the specified status.
- **`export [var=value]`**: Set environment variables.

## Advanced Features

- **Pipelines**: Use `|` to chain commands.
- **Redirections**:
  - `>`: Redirect output to a file.
  - `<`: Redirect input from a file.
  - `>>`: Append output to a file.
- **Heredoc**: Use `<<` to read input until a delimiter is encountered.

### Example:

```bash
echo "Hello, World!" | grep "Hello" > output.txt
cat output.txt
```

## Notes

- The shell replicates key behaviors of Bash but does not aim for full POSIX compliance.
- The implementation supports basic error handling but may not cover edge cases like advanced wildcard expansions or process substitutions.

## License

This project is open-source and available under the MIT License. Contributions are welcome!

---

Happy coding!
