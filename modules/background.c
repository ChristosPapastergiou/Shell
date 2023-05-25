#include "mysh.h"

int* background_process;    // Array that stores if a process must run on background. If yes then its 1 if not its 0
char** background_groups;   // Array to store the commands from argv

// Allocates memory for backgroung array
static void background_array(){
    background_process = (int*) mem_allocate(sizeof(int) * ARGV_BUFFER);
    background_groups = (char**) mem_allocate(sizeof(char*) * ARGV_BUFFER);
}

bool search_for_background(int argc, char** argv){
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], BACKGROUND) == 0){  // Check if there are and background commands 
            return true;
        }
    }
    return false;
}

int count_processes(int argc, char** argv){
    int background_processes = 0;
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], BACKGROUND) == 0){  // If a command has & it means it must be a background process
            background_processes++;     // Counting the total amount of those processes
        }
    }
    return background_processes;
}

char** find_background(int argc, char** argv){
    int j = 0;
    int value = 0;
    background_array();
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], BACKGROUND) == 0){
            background_process[value] = 1;  // Update that the command must run on background
            argv[i] = NULL;
            value++;
        }else{
            background_groups[j] = argv[i];
            j++;
        }
    }
    return background_groups;
}

bool check_if_background(int process){
    if(background_process[process] == 1){
        return true;
    }
    return false;
}

void free_background(){
    mem_free(background_process);   // Freeing the 
    mem_free(background_groups);    // allocated memory
}