# Minishell

Minishell is a minimalistic, Unix-like shell implemented in C, created as part of the curriculum at 42 School. The project is designed to emulate essential shell functionalities, enabling users to execute commands, manage environment variables, handle redirections, and implement piping, among other features.

## Features

- **Command Execution**: Executes built-in commands such as `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`. For other commands, the shell forks a process to execute them.
- **Variable Expansion**: Supports expanding environment variables, such as `$HOME` and `$PWD`, while handling cases with quotes or incomplete variable names.
- **Redirection Handling**: Manages input (`<`), output (`>`), and append (`>>`) redirections for commands.
- **Heredoc**: Implements heredoc (`<<`) functionality, allowing users to input multiple lines until a specified delimiter is reached.
- **Piping**: Allows piping between commands (e.g., `ls | grep .c`).
- **Signal Handling**: Manages signals, including `Ctrl+C`, `Ctrl+D`, and `Ctrl+\`, with appropriate behavior for each scenario.
- **Error Management**: Provides error handling for various cases, including syntax errors and permission issues.
- **Norm Compliance**: The code adheres to the 42 Norm, with function limits on line length, argument count, and structure.

## Project Structure

- `main.c`: Entry point for the shell.
- `ft_cmd.c`: Implements built-in command functions.
- `process_token.c`: Handles token processing, including quotes, variable expansion, and redirections.
- `do_comand.c`: Manages non-built-in commands and forking processes.
- Additional helper files for tokenization, parsing, redirections, and signal handling.

## Setup and Usage

### Prerequisites
- **Operating System**: Linux-based system or WSL (Windows Subsystem for Linux).
- **Compiler**: GCC or Clang.
  
### Compilation
To compile the shell, run the following command in the project directory:

```bash
make
```

This will generate an executable named `minishell`.

### Running Minishell
After compilation, start Minishell by running:

```bash
./minishell
```

From here, you can execute commands as you would in a typical shell.

## Key Notes

- **Built-in Commands**: The shell has a set of built-in commands. These commands are implemented within `ft_cmd` to handle actions like changing directories (`cd`) and setting environment variables (`export`).
- **Quotes**: Double and single quotes are handled to preserve whitespace or suppress variable expansion as needed.
- **Error Messages**: Minishell provides informative error messages, including syntax errors when an invalid command structure is entered.
- **Environment Variables**: Variables such as `$HOME`, `$PWD`, and custom variables are supported.
- **Heredoc with Delimiter**: In cases where a delimiter is not provided or `Ctrl+D` is pressed, the shell issues a warning message.

## Known Issues

- **Syntax Handling**: Certain complex cases involving heredoc and piping may have edge-case handling issues.
- **Signal Handling Limitations**: Although signals are managed, behaviors in certain combinations of signals may not align perfectly with standard shells.

## Future Improvements

- Implement advanced redirection and piping features.
- Refine signal handling for seamless interaction.
- Enhance variable expansion logic for complex cases.

## Contributing

If you're interested in contributing, feel free to fork this repository, make changes, and open a pull request. Contributions are welcome and encouraged to help improve the project!

## License

This project is for educational purposes as part of the 42 School curriculum and follows the 42 School's license.
