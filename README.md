# MangiShell

## Usage

1. make
2. ./a.out

## Assumptions

* Path sizes are at max 1024 character
* Max character limit of a executable is 256 characters
* History file should not be deleted during execution of the shell, it leads to a segmentation fault and it's not my fault.
* In the implementation of `jobs` , there can be at most 1022 background processes.

## Features

### `;` separated commands

- You can run more than 1 shell command in  a single line by separating them with a `;`

### ls

- Displays the contents of a directory if specified, else displays contents of current directory.
- Add `-l` to display the details of each file.
- Add `-a` to display the hidden contents.
- The directories and flags can be mentioned in any order after ls

### cd

- change the current working directory to the relative or the absolute path passed as argument.
- supports paths of the form `~/` , `./` , `../`
- `~` to go back to the shell executable directory, this acts as the home directory of the shell.
- `..` to go to the previous directory. If you go behind the home directory of shell, the absolute path is printed in the prompt instead.
- `-` will change your directory to the previous directory you were in.
- no arguments passed change the directory to home directory. 

### pwd

- To see the present working directory `pwd`.

### pinfo

- Displays the process information.
    - Process ID
    - Process Status
    - Virtual memory
    - executable path.
- If the PID is not mentioned it will show the information about the shell process.
- If PID does not exist, then error is thrown.

### Discover

- `discover` Prints all the contents of a directory in a recursive manner. If no path is specified, then the command runs on current directory.
- The paths are printed in alphabetical order, and in a breadth-first search manner.
- Flag `-f` may be used to list only all the file paths.
- Flag `-d` may be used to list only all the directory paths.

### History

- Displays the last 10 used commands with most recent ones appearing at the bottom of the list.
- Stores upto 20 history commands.
- Implemented using a hidden file named `.history.txt`

### File redirection

- `<` is used for input redirection. `cat < input.txt` File contents of input are redirected to stdin and cat is executed.
- `>`  is used for output redirection. `ls > output.txt` the output of ls is redirected to output file. This is also called the `truncate` mode, since it overwrites the file `output.txt` if it exists.
- `>>` is used for output redirection in `append` mode. It appends the data to the file it is redirected into.
- Both input and output redirection can be combined together in a single command.

### Piping

- `|` Can be used to redirect one commands output to another file. For instance, `ls | wc` Will redirect the output of ls to wc.
- More than 2 commands can be chained together for piping.
- Input Output redirection using `>`,`<` and `>>` can also be done in piping

### Jobs

- Jobs are processes spawned by the shell, `jobs` will list the background processes spawned by the shell along with their status and the pid in the order in which they were spawned.
- `jobs -r` will list all the running processes.
- `jobs -s` will list all the stopped processes.
- `signal job_number signal_number` Will send the signal to the given job.
- `exit` Will exit out of the shell.

### Signal handling

- `Ctrl+Z` will kill send the current foreground process to the background and will stop the process.
- `Ctrl+C` will interrupt the currently running foreground process, if no foreground process is present, it will do nothing.
- `Ctrl+D` Will Exit out of the shell.

### fg/bg

- `fg` can be used to bring a background process to the foreground process. The shell will only continue to execute after the process has terminated.
- `bg` can be used to make a stopped background process run again.
