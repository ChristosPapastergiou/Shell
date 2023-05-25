#include "mysh.h"

int search_pipe(int argc, char** argv){
    int pipe_point = 0;
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], PIPE) == 0){
            argv[i] = NULL;
            return i;  
        }
    }
    return pipe_point;  
}