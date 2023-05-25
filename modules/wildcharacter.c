#include "mysh.h"

int dup_counter = 0;

// Check if any argument has a wild character
static bool check_for_wild_character(int argc, char** argv){
    for(int i = 0; i < argc; i++){
        if((strchr(argv[i], WILD_STAR) != NULL) || (strchr(argv[i], WILD_QA) != NULL)){
            return true;
        }
    }
    return false;    
}

int search_wild(int* argc, char** argv){
    if(check_for_wild_character((*argc), argv) == false){
        return 0;                                   
    }

    glob_t buffer;
    dup_counter = 0;    // To zero...next time is not going to have the same amount of duplicates        
    int dup_flag = 0;   // Flag to save only the first point that duplicates started appear
    int dup_point = 0;  // The point that duplicates started appear
    bool first_element = true;  // If it is the first matching element then no need to increase argc etc
    for(int i = 0; i < (*argc); i++){
        if((strchr(argv[i], WILD_STAR) != NULL) || (strchr(argv[i], WILD_QA) != NULL)){
            if(glob(argv[i], 0, NULL, &buffer) == 0){
                for(int j = 0; j < buffer.gl_pathc; j++){
                    if(first_element == true){
                        argv[i] = strdup(buffer.gl_pathv[j]);       // Just duplicate to the existed position
                        first_element = false;                      // if it is the first element that matches
                        dup_counter++;
                        if(dup_flag == 0){
                            dup_flag = 1;
                            dup_point = i;
                        }
                        i++;
                    }else{
                        (*argc)++;  // argument counter must change
                        if((*argc) > ARGV_BUFFER){
                            printf("Too many arguments. Max %d\n", ARGV_BUFFER);    // Buffer check
                            return -1;
                        }

                        for(int j = (*argc); j > i; j--){       // Move all arguments one position
                            argv[j] = argv[j-1];
                        }
                        argv[i] = strdup(buffer.gl_pathv[j]);   // Finally duplicate the next match
                        dup_counter++;
                        i++;
                    }
                }

                first_element = true;   // Setup again the variable
                globfree(&buffer);
                --i;
            }
        }
    }
    return dup_point;
}

void remove_duplicates(int dup_point, int argc, char** argv){
    int total_frees = 0;
    if(dup_point != 0){
        for(int i = dup_point; i < argc; i++){
            if(argv[i] != NULL){
                if((strchr(argv[i], WILD_STAR) != NULL) || (strchr(argv[i], WILD_QA) != NULL)){ // Some arguments with no matching files
                    continue;                                                                   // will remain with the wild characters
                }else if(total_frees == dup_counter){   // Also the total amount of frees need to do are equal to the dup_counter
                    break;
                }else{
                    free(argv[i]);
                    total_frees++;
                }
            }
        }
    }
}