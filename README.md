# NeoShell: A *sh-like Shell for the Neo6502

A modern, usable, and extensible command-line shell for the Neo6502 computer, written in 6502 assembly.

## 📜 Overview

NeoShell aims to bring a more powerful and familiar command-line experience, similar to traditional Unix-like shells, to the Neo6502 platform. The primary goal is to make the system more "usable" for developers and power users by providing a robust environment for file system management, program execution, and system interaction.

## ✨ Features

### 🎯 Core Features

* **POSIX-like Experience:** Emulate the behavior and feature set of a standard `sh`-like shell wherever feasible on the 8-bit platform.

* **File System Management:** Full capability to navigate and manage the file system.

* **Program Execution:** The ability to launch external programs from the file system (e.g., from a `/bin` directory), with the shell terminating to cede control.

* **BASIC Integration:** Provide a command to drop from the shell into the BASIC interpreter.

### 🚀 Planned Features

* **Rich Built-in Commands:** A core set of essential utilities (`ls`, `mv`, `cp`, `cat`, `touch`).

* **External Commands:** Investigate a mechanism for adding new commands without recompiling the shell, potentially through a defined path or plugin system.

* **Inter-Process Communication (IPC):** Research methods for processes to communicate, which is a prerequisite for features like pipes.

* **Pipes and Redirects:** Implement the ability to pipe the output of one command into the input of another (`|`) and redirect output to files (`>`).

## 🚦 Current Status

The project is in the early stages of development. The core command parser and dispatcher are functional.

### ✅ Implemented Built-in Commands:

* `help`: Displays version information.

* `clear`: Clears the screen.

* `echo [args]`: Prints arguments to the screen.

* `ls`: Lists the currect directory contents

* `cd <dir>`: Changes to the directory given in the argument

* `mkdir <dir>`: Creates the directory given in the argument.

* `cwd`: Displays the current working directory.

* `cat [args]`: Outputs a file to the console.

* `cp <a> <b>`: Copies file <a> to file <b>.

* `exit`: Halts the shell.

---
*This README will be updated as the project progresses.*