# Minishell Project

Welcome to the **Minishell** project! This is a simple Unix shell implemented in C, created as part of the 42 School curriculum. The goal of this project is to replicate the behavior of a real shell, handling command execution, pipelines, redirections, and environment variables.

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Signal Handling](#signal-handling)
- [Environment Variables](#environment-variables)
- [Redirections and Pipes](#redirections-and-pipes)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [Acknowledgments](#acknowledgments)
- [License](#license)

## Features

- Command execution with arguments
- Support for environment variables and `$?` exit status
- Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Input and output redirections (`<`, `>`, `>>`)
- Pipes (`|`) to connect commands
- Signal handling for `Ctrl+C`, `Ctrl+D`, and `Ctrl+\`
- Command history using the Readline library
- Error handling similar to Bash

## Installation

### Prerequisites

- **C Compiler**: GCC or Clang
- **Make**: To build the project
- **Readline Library**: Required for command history and line editing

### Clone the Repository

```bash
git clone https://github.com/Mohamadaliibrahim/minishell.git
```

### Enter the files
```bash
cd minishell/minishell
```

### Build

Run the following command to compile the project:

```bash
make
```

This will generate an executable called `minishell`.

## Usage

Start the shell by running:

```bash
./minishell
```

You'll see a prompt:

```bash
HELL>
```

Now you can start typing commands just like in a regular shell.

## Built-in Commands

### `echo`

Displays a line of text. Supports the `-n` option to omit the trailing newline.

```bash
echo "Hello, World!"
echo -n "Hello without newline"
```

### `cd`

Changes the current directory.

```bash
cd /path/to/directory
cd ..
cd ~   # Goes to the HOME directory
cd -   # Goes to the previous directory
```

### `pwd`

Prints the current working directory.

```bash
pwd
```

### `export`

Sets environment variables.

```bash
export VAR=value
export PATH=$PATH:/new/path
```

### `unset`

Unsets environment variables.

```bash
unset VAR
```

### `env`

Displays the list of environment variables.

```bash
env
```

### `exit`

Exits the shell.

```bash
exit
exit 42   # Exits with status code 42
```

## Signal Handling

- **Ctrl+C** (`SIGINT`): Interrupts the current command and displays a new prompt.
- **Ctrl+D** (`EOF`): Exits the shell.
- **Ctrl+\** (`SIGQUIT`): Does nothing.

## Environment Variables

- Supports variable expansion using `$`.
- Special variable `$?` contains the exit status of the last command.
- Handles `$VAR` and `${VAR}` syntax.

## Redirections and Pipes

### Redirections

- Output redirection: `>`
- Input redirection: `<`
- Append output: `>>`

```bash
ls > file.txt
cat < file.txt
echo "New line" >> file.txt
```

### Pipes

Connects the output of one command to the input of another.

```bash
ls -l | grep ".c" | wc -l
```

## Project Structure

- `src/`: Contains the source code files.
- `inc/`: Header files.
- `Makefile`: Builds the project.
- `README.md`: This file.

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a new branch:

   ```bash
   git checkout -b feature-name
   ```

3. Commit your changes:

   ```bash
   git commit -m "Add new feature"
   ```

4. Push to the branch:

   ```bash
   git push origin feature-name
   ```

5. Open a pull request.

## Acknowledgments

- **42 School**: For providing the project guidelines.
- **GNU Bash**: Inspiration for shell features.
- **Readline Library**: For command history and line editing.

## License

This project is licensed under the MIT License.

---

**Author**: Mohammad Ali Ibrahim

- **LinkedIn**: [www.linkedin.com/in/mohammadali-ibrahim](https://www.linkedin.com/in/mohammadali-ibrahim)
- **GitHub**: [Mohamadaliibrahim](https://github.com/Mohamadaliibrahim)

---