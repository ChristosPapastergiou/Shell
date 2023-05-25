#include "mysh.h"

void shell(){
    printf("in-mysh-now:> ");
}

void exit_shell(Alias alias, History history, char* input, char** argv){
    save_history(history);

    mem_free(argv);
    mem_free(input);
    mem_free(alias);
    mem_free(history);

    exit(EXIT_SUCCESS);
}

void change_directory(char** argv){
    if(argv[1] == NULL){
        argv[1] = getenv("HOME");   // Home directory
        if(chdir(argv[1]) != 0){    // chdir doing the job automatically searching in PATH
            perror("chdir error");
        }
    }else{
        if(chdir(argv[1]) != 0){
            perror("chdir error");
        }
    }
}

Pointer mem_allocate(size_t size){
    Pointer array = calloc(1, size);    // calloc to initialize the values to NULL
    return array;
}

void mem_free(Pointer pointer){
    free(pointer);
}

void remove_quotes(int argc, char** argv){
    for(int i = 0; i < argc; i++){
        char* front;
        char* back;
        if((front = strchr(argv[i], QUOTES)) != NULL){
            memmove(front, front + 1, strlen(front + 1) + 1);   // +1 in size for the NULL terminator
        }
        if((back = strchr(argv[i], QUOTES)) != NULL){
            (*back) = '\0';
        }
    }
}

void parse_args(char* input, int* argc, char** argv){
    int i = 0;
    argv[i] = strtok(input, STRTOK_DELIMITER);     // Take the first input
    while(argv[i] != NULL){           // If there is another argument i will add it to the next position of the array
        i++;
        argv[i] = strtok(NULL, STRTOK_DELIMITER);  // Then just keep going to add arguments in the array untill its over

        if(i >= ARGV_BUFFER){
            printf("Too many arguments. Max Buffer size is : %d\n", ARGV_BUFFER);
            exit(EXIT_FAILURE);
        }
    }
    argv[i] = NULL;   // Just setting the last NULL

    (*argc) = i;
}

void exec_command_pipe(char** argv, int pipe_point){
    pid_t pid1, pid2;
    int redirection_point, in_fd, out_fd;

    /**** Pipe ****/

    int fd[2];
    if(pipe(fd) < 0){
        perror("pipe failure");
        exit(EXIT_FAILURE);
    }

    /**** Processes ****/
    
    if((pid1 = fork()) < 0){
        perror("fork failure");
        exit(EXIT_FAILURE);
    } 

    if(pid1 == 0){
        redirection_point = 1;
        if((in_fd = assign_in_file_descriptor(argv, &redirection_point)) < 0){
            return;
        }
        in_redirection(in_fd);  // Make sure it's taken care of redirection. Only < for the first child

        close(fd[READ]);                // First child will be the 
        dup2(fd[WRITE], STDOUT_FILENO); // input to the second one
        close(fd[WRITE]);
        
        execvp(argv[0], &argv[0]);      // execvp doing our job automatically (searching in PATH)

        printf("%s : command not found\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if((pid2 = fork()) < 0){
        perror("fork failure");
        exit(EXIT_FAILURE);
    }

    if(pid2 == 0){
        redirection_point = pipe_point; // Keep going from this point because the pipe symbol is NULL 
        if((out_fd = assign_out_file_descriptor(argv, &redirection_point)) < 0){
            return;
        }
        out_redirection(out_fd);        // Make sure it's taken care of redirection. Only > for the second child 

        close(fd[WRITE]);               // Second child will 
        dup2(fd[READ], STDIN_FILENO);   // take first's input
        close(fd[READ]);

        execvp(argv[pipe_point+1], &argv[pipe_point+1]);    // execvp doing our job automatically (searching in PATH)

        printf("%s : command not found\n", argv[pipe_point+1]);
        exit(EXIT_FAILURE);
    }

    close(fd[READ]);
    close(fd[WRITE]);
    while(wait(NULL) > 0);
}

void exec_command_background(int argc, char** argv){
    pid_t pid;
    int temp = 0;
    int jobpg = 0;
    int background_processes = count_processes(argc, argv);
    char** background_groups = find_background(argc, argv);

    for(int i = 0; i < background_processes; i++){
        if((pid = fork()) < 0){
            perror("fork failure");
            exit(EXIT_FAILURE);
        } 

        if(pid == 0){
            if(check_if_background(i) == true){
                setpgid(pid, jobpg);    // All children must belong to the process group of the first child
            }

            execvp(background_groups[temp], &background_groups[temp]);    // execvp doing our job automatically (searching in PATH)

            printf("%s : command not found\n", background_groups[temp]);
            exit(EXIT_FAILURE);
        }
        if(check_if_background(i) == true){
            if(jobpg == 0){         // If it is the first child then  set as
                jobpg = pid;        // the process group the pid of this first child
            }
            setpgid(pid, jobpg);    // All children must belong to the process group of the first child
        }
        while(waitpid(pid, NULL, WNOHANG) > 0);
        temp++;
    }   

    free_background();
}

void exec_command(char** argv){
    pid_t pid;
    int redirection_point, in_fd, out_fd;
    
    if((pid = fork()) < 0){
        perror("fork failure");
        exit(EXIT_FAILURE);
    } 

    if(pid == 0){
        redirection_point = 1;
        if((in_fd = assign_in_file_descriptor(argv, &redirection_point)) < 0){
            return;
        }
        if((out_fd = assign_out_file_descriptor(argv, &redirection_point)) < 0){
            return;
        }
        out_redirection(out_fd);    // Before exec come make sure
        in_redirection(in_fd);      // it's taken care of redirections

        execvp(argv[0], &argv[0]);  // execvp doing our job automatically (searching in PATH)

        printf("%s : command not found\n", argv[0]);
        exit(EXIT_FAILURE);
    }else{
        wait(NULL);
    }
}