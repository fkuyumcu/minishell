# minishell

A minimal UNIX shell implementation written in C.  
This project is part of the 42 curriculum and aims to replicate core functionalities of Bash, focusing on process management, input parsing, environment variables, and execution.

## 🧠 Project Overview

**minishell** is a simplified shell that interprets and executes user commands. It supports pipelines, redirections, built-in commands, and basic signal handling — mimicking the behavior of a real shell while maintaining a clean and modular codebase.

## ✨ Features

- Prompt display and user input
- Command execution (with absolute/relative paths)
- Built-in commands:
  - `echo`
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- Input redirection: `<`
- Output redirection: `>` and `>>`
- Here-document (`<<`)
- Pipes (`|`)
- Environment variable expansion (`$VAR`)
- Quote handling (single and double quotes)
- Signal handling (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)
- Exit status management
- Error handling with descriptive messages

## 🛠️ Installation

```bash
git clone https://github.com/yourusername/minishell.git
cd minishell
make
./minishell
```

## ✅ Usage

Once launched, the shell waits for user input:

```bash
minishell$ echo Hello World
Hello World
```

Supports chaining with pipes:

```bash
minishell$ cat file.txt | grep keyword | wc -l
```

Redirection example:

```bash
minishell$ echo "output" > file.txt
```

## 📁 Project Structure

```
minishell/
├── src/            # Source files
├── includes/       # Header files
├── libft/          # Custom libc functions (42's libft)
├── Makefile
└── README.md
```

## 📌 Constraints

- No usage of `system()` or other shell-based functions.
- Only allowed to use specified libc functions.
- Must manage memory carefully (no leaks).
- Must mimic behavior of bash where applicable.


## 🚦 Signals

| Signal     | Behavior               |
|------------|------------------------|
| `Ctrl+C`   | Interrupts prompt      |
| `Ctrl+D`   | Exits shell (EOF)      |
| `Ctrl+\`  | Ignored / managed      |

## 🔍 Known Issues / Limitations

- No support for advanced scripting features like `for` loops or `if` statements.
- Not all Bash behaviors are fully replicated.

## 🧑‍💻 Authors

- [fkuyumcu](https://github.com/fkuyumcu)
- [yunuseus](https://github.com/yunuseus)

## 📄 License

This project is for educational purposes and follows the 42 school's guidelines.

---

> *"Building a shell teaches you more about UNIX than reading a book ever could."*
