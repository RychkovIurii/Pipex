# Pipex Project

## Overview
The **Pipex** project simulates the functionality of pipes and redirections in a shell environment, allowing multiple commands to be chained together.  
This program mimics the behavior of the following shell command:

```bash
< file1 cmd1 | cmd2 > file2
```

It processes the given files and shell commands, handling input/output redirections and piping multiple commands.  
Additionally, the bonus part of the project includes support for handling multiple pipes and the here_doc feature for custom delimiters.

---

## Project Details
- Total Time Spent: 46 days
- Total Lines of Code: 1276
- Commits: 49
- Programming Language: C

**Main Features**:
- Handling input/output redirections
- Piping multiple commands
- Error handling improvements
- Support for "here_doc" and multiple pipes (bonus)

---

## Challenges
1. **Parsing**  
The core challenge was parsing the input commands and handling the pipe mechanism. To simplify the parsing process, I opened a new instance of the shell using the -c flag to execute the commands. This allowed for an easier parsing flow and correct execution.

2. **Random Error Sequences**  
Shell commands sometimes produce errors that are not in a consistent order, depending on the speed of execution. To solve this, I used pointer address of main struct to create unique names for the error logs. I then stored errors in a personal error file and read them sequentially to ensure they appear in the correct order.

---

## Allowed Functions
open, close, read, write, malloc, free, perror, strerror, access, dup, dup2, execve, exit, fork, pipe, unlink, wait, waitpid, ft_printf.

---

## Features
Mandatory Part:  
The program takes four arguments: file1, cmd1, cmd2, and file2.  
It mimics the following shell command:
```bash
$> < file1 cmd1 | cmd2 > file2
```
---

## Bonus Part:
- Multiple Pipes: The program can handle a chain of multiple commands and pipes.
- Here Document: The program supports here_doc functionality when the first parameter is `here_doc`, allowing you to specify a delimiter and use it in the input.

---

## Files
`pipex.c`:  The main implementation of the program.  
`pipex_bonus.c`:Bonus part implementation, including multiple pipes and here_doc support.  
`Makefile`: Used to compile the project, following the required rules and avoiding relinking.  
`pipex.h`: Contains function declarations and necessary constants.  

---

## Requirements
- External functions: open, close, read, write, malloc, free, perror, strerror, access, dup, dup2, execve, exit, fork, pipe, unlink, wait, waitpid, and any equivalent functions.  
- Memory Management: The project ensures that all heap-allocated memory is properly freed, and there are no memory leaks.  
- Error Handling: Extensive error checks are implemented, including unique error logs for tracking and resolving issues.  

---

## How to Run
Clone the repository:

```bash
git clone https://github.com/RychkovIurii/Pipex.git
```
Compile the project using the Makefile:

```bash
make
```

Compile the bonus part using the Makefile:

```bash
make bonus
```

Run the program:

```bash
./pipex file1 cmd1 cmd2 file2
```
---

## Bonus Features
The bonus features include:
- Handling multiple pipes.  
- Supporting here_doc for custom input delimiters.  

Run the program with multiple commands and pipes:
```bash
./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
```

For the here_doc feature:
```bash
./pipex here_doc LIMITER cmd cmd1 file
```
