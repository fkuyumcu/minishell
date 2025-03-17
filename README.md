# Minishell

## Overview
Minishell is a simplified shell implementation written in C that mimics the behavior of a Unix shell. It supports basic shell functionalities, including command execution, pipes, redirections, and built-in commands.

## Features
- Command execution with arguments
- Built-in commands:
  - `cd`
  - `echo`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- Input and output redirections (`>`, `>>`, `<`)
- Piping (`|`)
- Environment variable handling
- Signal handling (Ctrl+C, Ctrl+D, Ctrl+\)

## Installation
```bash
# Clone the repository
git clone https://github.com/yourusername/minishell.git
cd minishell

# Compile the project
make
```

## Usage
Run Minishell by executing:
```bash
./minishell
```
You can then enter commands just like in a standard shell.

## Example Commands
```bash
ls -l | grep minishell
cat file.txt > output.txt
echo "Hello, World!"
export MY_VAR=value
echo $MY_VAR
```

## Dependencies
- GNU Make
- GCC
- POSIX-compliant system (Linux or macOS)

## Authors
-(@fkuyumcu)

-(@yunuseus)

## License
This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## Acknowledgments
Inspired by the Unix shell and various shell scripting resources.

