#include "mysh.h"

// Make history with a struct and delete global!
struct history{
    int history_counter;    // Counter for the total amount of commands in the array
    char history[HISTORY_BUFFER][INPUT_BUFFER]; // Array to store the last HISTORY_BUFFER commands
}; 

// Checks if the last saved command is the same as the new command
static bool last_from_history(History history, const char* input){
    if(strcmp(input, history->history[history->history_counter-1]) != 0){ // Just compare the last array's command with the last input from user
        return true;
    }
    return false;
}

History create_history_array(){
    History history = (History) mem_allocate(sizeof(struct history) * HISTORY_BUFFER);
    history->history_counter = 0;
    return history;
}

void save_history(History history){
    FILE* file = fopen("history.csv", "w");
    if(file != NULL){
        for(int i = 0; i < history->history_counter; i++){
            fprintf(file, "%s\n", history->history[i]);
        }
        fclose(file);
    }
}

void load_history(History history){
    FILE* file = fopen("history.csv", "r");
    if(file != NULL){
        char commands[HISTORY_BUFFER];
        while(fgets(commands, sizeof(commands), file) != NULL){
            commands[strcspn(commands, "\n")] = '\0';
            add_to_history(history, commands);
        }
        fclose(file);
    }
}

void add_to_history(History history, char* input){
    if(last_from_history(history, input) == true){
        if(history->history_counter < HISTORY_BUFFER){           // If the array is not full
            strcpy(history->history[history->history_counter], input);    // just save the command and
            history->history_counter++;                          // prepare for the next command
        }else{
            for(int i = 0; i < HISTORY_BUFFER - 1; i++){    // If the array is full all the commands
                strcpy(history->history[i], history->history[i+1]);           // will be at -1 position and the new one
            }                                               // will be added at the last position
            strcpy(history->history[HISTORY_BUFFER-1], input);    
        }
    }
}

char* get_from_history(History history, int history_number){
    if((history_number > 0) && (history_number <= history->history_counter)){    // Number must be a logic number within boundaries
        return history->history[history_number-1];
    }
    return NULL;
}

void print_history(History history){
    for(int i = 0; i < history->history_counter; i++){
        printf("%d : %s\n", i+1, history->history[i]);
    }
    printf("[Selection 1-%d]\n", history->history_counter);
}