
# C-Shell: A Feature-Rich Linux Shell Implementation in C

Welcome to C-Shell, a powerful and customizable Unix shell implemented in the C programming language. This project showcases the versatility of system calls in Linux, offering a robust and user-friendly shell experience.

# Features:

* System Calls Galore: Leverage the strength of various Linux system calls to achieve efficient process management, file manipulation, and I/O operations.

* Interactive Command Line: Enjoy an interactive command-line interface that supports a wide array of commands, providing a seamless user experience.

* Customization Options: Tailor the shell to your needs with customizable configurations, aliases, and prompt settings.

* Process Management: Efficiently handle processes with built-in functionalities for background processes, job control, and signal handling.

* File Operations: Leverage file-related system calls for file manipulation, redirection, and piping to enhance the shell's file-handling capabilities.

# Getting Started: 
* Clone the Repository: \
 `git clone https://github.com/your-username/C-Shell.git`\
`cd C-Shell`

* Compile and Run: \
`make` \
`./a.out`

# Description

- **header.h** contains all the header files being used in this codebase along with global variables/arrays which are being used using extern to use it in different files.
- **main.c** is the main C file for this shell from where all the different functions get called accordingly.
- **call.c** and **call.h** are used to call other functions.
- **prompt.c** and **prompt.h** handles the display of command prompt in the following format:
  ```
  <Username@SystemName:~>
  ```
- **warp.c** and **warp.h** handles changing the directory that the shell is currently in. It also prints the full path of working directory after changing. Follwing flags are also implemented with this:
`.`, `~`, `..`, `-`.
- **proclore.c** and **proclore.h** is used to obtain information regarding a process. If an argument is missing, print the information of your shell.
- **pastevents.c** and **pastevents.h** handle the storing of previous command, so that user can access the past 15 commands run on shell and also execute then. **pastevents.txt** stores those commands.
- **peek.c** and **peek.h** function like the `ls` command in terminal. It lists all the files and directories in the specified directories in lexicographic order. It suppors the following flags:
```
-l : displays extra information
-a : displays all files, including hidden files
```
- **seek.h** and **seek.c** looks for a file/directory in the specified target directory (or current if no directory is specified). It returns a list of relative paths (from target directory) of all matching files/directories (files in green and directories in blue) separated with a newline character. 
- **foreground.c** and **foreground.h** handle the system foreground processes, such as ```sleep```, ```echo```, etc. 
- **background.c** and **background.h** handle the system background processes.

# Assumptions
- paths/names will not contain any whitespace characters.
- pastevents.txt is already created. The above codebase does not create a new file if its not present. It assumes that the textfile is present already.
- Shell is exited when you run the command "exit" and it is saved in pastevents.txt.
- With peek if argument is provided, it is always a directory (not a file).
- There won't be more than 4096 files/directories in a directory.
- For seek, executables are not displayed. 
- Path length of any path won't exceed 200 characters.
- There won't be more than 4096 processes running in the background.
