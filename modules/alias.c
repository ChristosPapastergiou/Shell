#include "mysh.h"

struct alias{
    int argc_counter;   // Saves the number of arguments that have been matched to an alias
    int alias_counter;  // Saves the total amount of existing aliases
    char alias[ALIAS_BUFFER];   // Alias is the "new" command
    char command[ALIAS_BUFFER][INPUT_BUFFER]; // And the command that will be "changed" (2D because some may be composed by more than one argument)
};

// Returns the number of arguments that have been matched to an alias
static int arguments_to_alias(Alias alias, int i){
    return alias[i].argc_counter;
}

// Prints all the aliases a shell has
static void print_alias(Alias alias){
    for(int i = 0; i < alias->alias_counter; i++){
        printf("alias %s =", alias[i].alias);
        for(int j = 0; j < arguments_to_alias(alias, i); j++){
            printf(" %s", alias[i].command[j]);
        }
        printf("\n");
    }
}

// Passing to alias array the alias and command user gave
// Referred to in the case with one argument as the command
static void few_argument_pass(Alias alias, int argc, char** argv){
    for(int i = 0; i < alias->alias_counter; i++){
        if(strcmp(argv[1], alias[i].alias) == 0){   // Check if an alias is already inside so go change it
            strcpy(alias[i].alias, argv[1]);        // argv[1] is the alias
            strcpy(alias[i].command[0], argv[2]);   // argv[2] is the command
            alias[i].argc_counter = 1;
            return;
        }
    }
    strcpy(alias[alias->alias_counter].alias, argv[1]);
    strcpy(alias[alias->alias_counter].command[0], argv[2]);
    alias[alias->alias_counter].argc_counter = 1;
    alias->alias_counter++;
}

// Passing to alias array the alias and commands user gave
// Referred to in the case with more than one argument as the command
static void many_arguments_pass(Alias alias, int argc, char** argv){
    if((argc - 2) > ALIAS_BUFFER){
        printf("Too many arguments. Max alias buffer %d\n", ALIAS_BUFFER);
        return;
    }
    for(int i = 2; i < argc; i++){                          // 1st -> createalias, 2nd -> alias, so thats why i = 2
        for(int j = 0; j < alias->alias_counter; j++){
            if(strcmp(argv[1], alias[j].alias) == 0){       // Check if an alias is already inside so go change it
                strcpy(alias[j].alias, argv[1]);            // argv[1] is the alias
                for(int k = 0; k < argc - 2; k++){
                    strcpy(alias[j].command[k], argv[k+2]); // argv[k+2] is the command
                }
                alias[j].argc_counter = argc - 2;
                return;
            }
        }
        strcpy(alias[alias->alias_counter].alias, argv[1]);
        for(int j = 0; j < argc - 2; j++){
            strcpy(alias[alias->alias_counter].command[j], argv[j+2]);
        }
        alias[alias->alias_counter].argc_counter = argc - 2;
    }
    alias->alias_counter++;
}

Alias create_alias_array(){
    Alias alias = (Alias) mem_allocate(sizeof(struct alias) * ALIAS_BUFFER);
    alias->alias_counter = 0;
    return alias;
}

void create_alias(Alias alias, int argc, char** argv){
    if((argv[1] != NULL) && (argv[2] != NULL)){
        if(alias->alias_counter < ALIAS_BUFFER){
            if(argc <= 3){
                few_argument_pass(alias, argc, argv);   // Call function to save the alias and the command
            }else{
                many_arguments_pass(alias, argc, argv); // Call function to save the alias and the commands
            }
        }else{
            printf("Max alias support %d\n", ALIAS_BUFFER);
        }
    }else{
        print_alias(alias);
        printf("createalias: usage: createalias alias[name] [name]\n");
    }
}

void delete_alias(Alias alias, char** argv){
    if(argv[1] != NULL){
        for(int i = 0; i < alias->alias_counter; i++){
            if(strcmp(argv[1], alias[i].alias) == 0){   // argv[1] the alias to delete
                for(int j = i; j < alias->alias_counter; j++){ // Push all the aliases one position
                    alias[j] = alias[j+1];
                }
                alias->alias_counter--;
                return;
            }
        }
    }else{
        print_alias(alias);
        printf("destroyalias: usage: destroyalias alias[name]\n");
    }
}

void search_alias(Alias alias, int* argc, char** argv){
    for(int i = 0; i < alias->alias_counter; i++){
        if(strcmp(argv[0], alias[i].alias) == 0){   // If input is an alias i will pass
            if(arguments_to_alias(alias, i) == 1){
                argv[0] = alias[i].command[0];
            }else{
                if((*argc) < 2){
                    for(int j = 0; j < arguments_to_alias(alias, i); j++){
                        argv[j] = alias[i].command[j];
                    }
                }else{
                    int temp_value = 0;
                    char** temp = (char**) mem_allocate(sizeof(char*) * ARGV_BUFFER);   // Term array to save from argv[1] until the end
                    
                    for(int j = 0; j < (*argc) - 1; j++){
                        temp[j] = argv[j+1];
                    }

                    (*argc) = 0;    // Argument count will change
                    for(int j = 0; j < arguments_to_alias(alias, i); j++){
                        argv[j] = alias[i].command[j];
                        temp_value++;
                        (*argc)++;
                    }

                    int k = 0;
                    while(temp[k] != NULL){
                        argv[temp_value] = temp[k]; // Passing the rest of the commands back to main argument array 
                        temp_value++;
                        (*argc)++;
                        k++;
                    }
                
                    mem_free(temp);
                }
            }
        }
    }
}