# Operating-System
1. Builded a simple **linux-like shell using C**. On executing your code, the shell start as:
MTL458:~$
where “ ~” is the path where we are currently at. The shell should terminate on pressing
Ctrl+C. An incorrect number of arguments or incorrect command format print an error
in the shell. The shell not crash and simply move on to the next command. The
shell execute simple commands (commands which are readily available as
executables and can be invoked using exec system calls) like ls, cat, echo, sleep. I assume that the user input does not include any extra functionalities like
background execution, pipes, or I/O redirection.

2. Implemented the “cd” command. Treated the directory from where the shell script is run as
home. I am able to go both forward and backward in directories including multiple
steps. E.g. If we go to “dir1” from the home directory we see the following.
MTL458:~/dir1$

3. Implemented the “history” command. It display the last 5 commands executed on
the shell. If there’s only one command executed since the start of the shell, it show
only one command in history and so on.
