#ifndef MYSH
#define MYSH

#define _GNU_SOURCE // Just for sigaction struct (without -> incomplete type)

/**** Includes ****/

#include <glob.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

/**** Defines ****/

#define PIPE "|"
#define QUOTES '"'
#define WILD_QA '?'
#define WILD_STAR '*'
#define BACKGROUND "&"
#define IN_REDIRECTION "<"
#define OUT_REDIRECTION ">"
#define DOUT_REDIRECTION ">>"
#define STRTOK_DELIMITER " \t"
#define SHELL_ESCAPE " \t\n\r"

#define READ 0 
#define WRITE 1
#define ALIAS_BUFFER 20
#define ARGV_BUFFER 128
#define INPUT_BUFFER 1024
#define HISTORY_BUFFER 20

#define EXIT_COMMAND "exit"    
#define CHANGE_DIRECTORY "cd"
#define HISTORY_COMMAND "history"
#define CREATE_ALIAS "createalias"
#define DESTROY_ALIAS "destroyalias"

/**** Read-Write Permissions ****/

#define READ_PERMS (O_RDONLY)
#define WRITE_PERMS_SINGLE (O_WRONLY | O_CREAT | O_TRUNC)   // Redirection > must wipe the file (if alredy exists) and write from the begging
#define WRITE_PERMS_DOUBLE (O_WRONLY | O_CREAT | O_APPEND)  // Redirection >> must keep the existing content (if already exists) and continue the writing

/**** Typedefs ****/

typedef void* Pointer;
typedef struct alias* Alias;
typedef struct history* History;

/**** command.c ****/

// Printing prompt
void shell();

// Execution of "exit" command. Frees all allocated space
void exit_shell(Alias alias, History history, char* input, char** argv);

// Execution of "cd" command. If no arguments back to home directory
void change_directory(char** argv);

// Allocates memory for Pointer with size size
Pointer mem_allocate(size_t size);

// Frees memory of the Pointer 
void mem_free(Pointer pointer);

// Removes quotes of the arguments. Arguments must be "clear" 
void remove_quotes(int argc, char** argv);

// Parsing the input inside the argv. Returns the number of arguments
void parse_args(char* input, int* argc, char** argv);

// In case input has a pipe, then execution of command
void exec_command_pipe(char** argv, int pipe_point);

// In case input has background, then exection of command
void exec_command_background(int argc, char** argv);

// Execution of a command
void exec_command(char** argv);

/**** history.c ****/

// Creates array to store commands as the history
History create_history_array();

// Saving the history to a history.txt
void save_history(History history);

// Loads the existed history
// If there is not nothing will be loaded
void load_history(History history);

// Adding to history the last command. If last saved command is
// the same as the last input it is not going to be saved again
void add_to_history(History history, char* input);

// Returns the command that user wants from the history [Selection with numbers]
char* get_from_history(History history, int history_number);

// Prints the history so user can see the options
void print_history(History history);

/**** redirection.c ****/

// Matching the file descriptor of in redirection (<). Returns -1 in case of failure
int assign_in_file_descriptor(char** argv, int* redirection_point);

// Matching the file descriptor of out redirection (> or >>). Return -1 in case of failure
int assign_out_file_descriptor(char** argv, int* redirection_point);

// Duplicates using out redirect (> or >>) 
void out_redirection(int out_fd);

// Duplicates using in redirect (<)
void in_redirection(int in_fd);

/**** wildcharacter.c ****/

// Building argument array with all the related files with the wild characters 
// Returns the buffer of glob() to be able to free at the end
int search_wild(int* argc, char** argv);

// Wild uses strdup. It makes clones so need to free the memory of duplicates
void remove_duplicates(int dup_point, int argc, char** argv);

/**** alias.c ****/

// Creats array that holds aliases
Alias create_alias_array();  

// Creates an alias based on 2nd & 3rd argument
// If alias already exists then it will be replaced with the new one
void create_alias(Alias alias, int argc, char** argv);

// Destroy an alias based on 2nd argument
void delete_alias(Alias alias, char** argv);

// Searching if alias exists. If yes it  
// will change the "fake" command with the "real" one.
void search_alias(Alias alias, int* argc, char** argv);

/**** pipe.c ****/

// Searching if there is any pipe symbol in arguments "|"
// If yes returns the position of the pipe. If no then 0
int search_pipe(int argc, char** argv);

/**** background.c ****/

// Searching if there is any background symbol in arguments "&"
// If yes returns the number of the processes that must run background. Returns 0 if no background
bool search_for_background(int argc, char** argv);

// Returns how many processes must run on the background
int count_processes(int argc, char** argv);

// Finds background processes and returning and array with the commands
char** find_background(int argc, char** argv);

// Returns if a process must run on background
bool check_if_background(int process);

// Frees memory allocated for background
void free_background();

#endif