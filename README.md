# Minishell

Minishell is a simple Unix shell project developed as part of the 42 School curriculum. This shell aims to replicate the basic functionality of popular shells like `bash`, providing command execution, built-in commands, redirections, piping, environment variable expansion, and more.

## Table of Contents
- [Features](#features)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Functionality Details](#functionality-details)
- [Error Handling](#error-handling)
- [Project Structure](#project-structure)
- [Norminette Compliance](#norminette-compliance)
- [Acknowledgments](#acknowledgments)

## Features

Minishell implements various features to offer a similar experience to other Unix shells:
- Command execution with arguments
- Redirection (`>`, `<`, `>>`, `<<`)
- Pipes (`|`) for chaining commands
- Environment variable expansion (e.g., `$HOME`)
- Signal handling (Ctrl+C, Ctrl+\)
- Heredoc with delimiter
- Basic built-in commands (`echo`, `cd`, `env`, `export`, `unset`, `pwd`, `exit`)
  
## Getting Started

### Prerequisites
- **Compiler**: GCC or Clang
- **Operating System**: Unix-based (Linux or macOS)

### Installation

Clone the repository and navigate to the project directory:

```bash
git clone https://github.com/yourusername/minishell.git
cd minishell
```

Then, compile the shell with:
```bash
make
```

This will generate the `minishell` executable.

### Running the Shell

To start the shell, run:
```bash
./minishell
```

## Usage

Once you start `minishell`, you can type commands as you would in a standard shell. Here are a few examples:

```bash
# Running a simple command
$ ls -l

# Using pipes
$ ls | grep "filename"

# Redirecting output to a file
$ echo "Hello, Minishell!" > hello.txt

# Using environment variables
$ echo $HOME

# Running built-in commands
$ cd /path/to/directory
$ export MY_VAR=value
$ unset MY_VAR
```

## Built-in Commands

The following built-in commands are implemented in Minishell:

- **`echo`**: Prints arguments to the standard output
- **`cd`**: Changes the current directory
- **`env`**: Displays the environment variables
- **`export`**: Adds or modifies environment variables
- **`unset`**: Removes environment variables
- **`pwd`**: Prints the current working directory
- **`exit`**: Exits the shell

Each command follows a specific implementation to replicate standard shell behavior as closely as possible.

## Functionality Details

- **Redirection**: Supports input and output redirection (`<`, `>`, `>>`), allowing commands to read from or write to files.
- **Piping**: Enables chaining commands, where the output of one command serves as the input to the next.
- **Heredoc**: Implements a heredoc (`<<`) for multiline input, supporting a custom delimiter.
- **Variable Expansion**: Expands variables like `$HOME`, `$USER`, `$PWD`. Invalid or undefined variables expand to an empty string.
- **Signal Handling**: Catches signals such as `SIGINT` and `SIGQUIT` to manage shell interruption (e.g., Ctrl+C handling).
  
## Error Handling

Minishell provides informative error messages similar to `bash`. Errors include:
- Command not found
- Syntax errors (e.g., invalid use of pipes or redirections)
- Permission errors
- Undefined variable expansion errors

## Project Structure

Here's a brief overview of the project's structure:

```plaintext
minishell/
├── src/
│   ├── builtins/               # Built-in commands implementations
│   ├── exec/                   # Command execution functions
│   ├── parser/                 # Tokenization and parsing logic
│   ├── signals/                # Signal handling functions
│   ├── main.c                  # Entry point
│   ├── ft_cmd.c                # Built-in command handling
│   ├── process_token.c         # Token processing and expansion
│   └── ...
├── include/
│   ├── minishell.h             # Main header file
│   └── ...
├── Makefile                    # Compilation instructions
└── README.md                   # Documentation
```

Each component serves a specific purpose to maintain modularity and clarity.

## Norminette Compliance

This project adheres to the 42 Norminette style guide:
- Functions are under 25 lines and use fewer than 4 arguments.
- Proper handling of file descriptors and memory management.
- Modular code structure for readability and maintainability.

## Acknowledgments

This project was developed as part of the 42 School curriculum, which encourages a deep understanding of Unix systems and shell behavior. Special thanks to the 42 community for their support and resources.
