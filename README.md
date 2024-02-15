# Shell

Implementation of a Shell in C

A basic implementation of a shell. The goal of this task is to familiarize you with the creation and management of processes and files and signals, 
writing a simple mysh shell (my simple shell) in a Linux environment and in the C programming language.

# Compilation & Run

    compile : make
    run     : ./mysh
          
# Support

- Alias             : Can create aliases to represent a command with a name
- Wild characters   : Can search files using wild characters and the matched file will show
- Pipe              : Can use pipe to give the first command as the input for the second command 
- Saved history     : Can save the history, bring one of the last 20 commands and execute
- Redirections      : Can give to commands input, output or both  
- Signal handling   : Ctrl-C & Ctrl-Z terminates and stops the current command with leaving the shell
- Background        : Can run some processes on the background and the same time foreground
- Simple commands   : All the basic commands of the bash (ls, cat etc...)

# Command examples

- Alias :          
    - To create an alias : createalias [name of alias] [name of command]
    - To destroy an alias : destroyalias [name of alias]
    - Print all aliases : createalias/destroyalias
- Wild characters :
    - Use * or ? like this : ls example*.t?t, cat example* etc
- Pipe :    
    - Pipe with reiderections : cat < example.txt | sort > example2.txt
    - Pipe with no redirections : ls -l | wc -l
- Saved history :
    - Access to history : history
    - Pick a command from history : Just type a valid number
- Redirections :
    - In redirection (<) : cat < example.txt
    - Out redirection (>) : cat > example.txt
    - Double out redirection (>>) : cat >> example.txt
    - Combine redirections : cat < example.txt > example2.txt
- Signal handling :
    - Stop process : Ctrl-C
    - Interrupt process : Ctrl-Z 
- Background processes :
    - Use & symbol : cat < example.txt & ls &
- Simple and basic bash commands :
    - Any command of the bash : ls, cd, cat, echo, man etc
