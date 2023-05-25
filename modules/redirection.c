#include "mysh.h"

int assign_in_file_descriptor(char** argv, int* redirection_point){
    int in_fd = 0;
    for(int i = (*redirection_point); argv[i] != NULL; i++){
        if(strcmp(argv[i], IN_REDIRECTION) == 0){
            argv[i] = NULL;                         // Need to remove the "<" from the list before execvp. Exevp must see only arguments not symbols
            in_fd = open(argv[i+1], READ_PERMS);    // Assign file desc for read mode
            (*redirection_point) = i;
            if(in_fd < 0){
                perror("Open failure");
                return in_fd;   // In this case shell will start over
            }
        }
    }
    return in_fd;
}

int assign_out_file_descriptor(char** argv, int* redirection_point){
    int out_fd = 0;
    for(int i = (*redirection_point) + 1; argv[i] != NULL; i++){    // For the next redirection must see after the NULL in redirection (<) made 
        if(strcmp(argv[i], OUT_REDIRECTION) == 0){
            argv[i] = NULL;                                     // Need to remove the ">" from the list before execvp. Exevp must see only arguments not symbols
            out_fd = open(argv[i+1], WRITE_PERMS_SINGLE, 0666); // Assign file desc for write mode (perms are for deletion and rewriting to the file)
            (*redirection_point) = i;
            if(out_fd < 0){
                perror("Open failure");
                return out_fd;  // In this case shell will start over
            }
        }else if(strcmp(argv[i], DOUT_REDIRECTION) == 0){
            argv[i] = NULL;                                     // Need to remove the ">>" from the list before execvp. Exevp must see only arguments not symbols
            out_fd = open(argv[i+1], WRITE_PERMS_DOUBLE, 0666); // Assign file desc for write mode (perms are for writing to the existing lines of file)
            (*redirection_point) = i;
            if(out_fd < 0){
                perror("Open failure");
                return out_fd;  // In this case shell will start over
            } 
        }
    }
    return out_fd;
} 

void in_redirection(int in_fd){
    if(in_fd != 0){
        close(0);                   // Need to close 0 file desc to be safe
        if(dup2(in_fd, 0) < 0){     // Using dup2 to redirect the standard output
            perror("dup2 failure");
            exit(EXIT_FAILURE);
        }
        close(in_fd);
    }
}

void out_redirection(int out_fd){
    if(out_fd != 0){
        close(1);                   // Need to close 1 file desc to be safe
        if(dup2(out_fd, 1) < 0){    // Using dup2 to redirect the standard output
            perror("dup2 failure");
            exit(EXIT_FAILURE);
        }
        close(out_fd);
    }
}