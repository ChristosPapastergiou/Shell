#include "mysh.h"

void signal_handler(int signum){}

int main(){
    /**** Data ****/

    Alias alias = create_alias_array(); // Alias array must remain until the end of the shell
    History history = create_history_array(); // History array must remain until the end of the shell

    char* command;
    int argc, history_number, wild, pipe_point;

    /**** Signal handling (ctrl-c & ctrl-z) ****/

    static struct sigaction act;
    act.sa_handler = signal_handler;
	sigfillset(&(act.sa_mask));
	sigaction(SIGINT, &act, NULL);      // SIGINT for ctrl-c
    sigaction(SIGTSTP, &act, NULL);     // SIGTSTP for ctrl-z

    /**** History ****/

    load_history(history);

    /**** Begging of the shell ****/

    while(1){
        shell();

        /**** Input ****/
        
        char* input = (char*) mem_allocate(sizeof(char) * INPUT_BUFFER);    // Array to keep user's input
        char** argv = (char**) mem_allocate(sizeof(char*) * ARGV_BUFFER);   // Array i will need to parse arguments

        fgets(input, INPUT_BUFFER, stdin);
        input[strcspn(input, "\n")] = '\0';

        if(input == NULL || strlen(input) == 0 || strspn(input, SHELL_ESCAPE) == strlen(input)) {
            mem_free(input);
            mem_free(argv);
            continue;
        }

        /**** History ****/

        if(sscanf(input, "%d", &history_number) == 1){  // If user gave a number then sscanf returns 1
            command = get_from_history(history, history_number);
            if(command != NULL){    
                strcpy(input, command);
            }else{
                printf("Invalid command selection\n");
                mem_free(input);
                mem_free(argv);
                continue;
            }
        }
        add_to_history(history, input);

        /**** Input parsing ****/

        parse_args(input, &argc, argv);     // 1st step: parse all the arguments 
        remove_quotes(argc, argv);          // 2nd step: remove all the " of the arguments (echo hello world = echo "hello world")

        /**** Alias ****/

        search_alias(alias, &argc, argv);   // 3rd step: Check for alias. Alias can be used with wild, background, pipe etc

        /**** Wild characters ****/

        wild = search_wild(&argc, argv);    // 4th step : Check for wild characters
        if(wild == -1){
            mem_free(input);
            mem_free(argv);
            continue;
        }

        /**** Command execution ****/

        if(strcmp(argv[0], EXIT_COMMAND) == 0){
            exit_shell(alias, history, input, argv);
        }else if(strcmp(argv[0], CREATE_ALIAS) == 0){
            create_alias(alias, argc, argv);
        }else if(strcmp(argv[0], DESTROY_ALIAS) == 0){
            delete_alias(alias, argv);
        }else if(strcmp(argv[0], HISTORY_COMMAND) == 0){
            print_history(history);
        }else if(strcmp(argv[0], CHANGE_DIRECTORY) == 0){
            change_directory(argv);
        }else if(search_for_background(argc, argv) == true){
            exec_command_background(argc, argv);
        }else if((pipe_point = search_pipe(argc, argv)) != 0){
            exec_command_pipe(argv, pipe_point);
        }else{
            exec_command(argv);     
        }

        remove_duplicates(wild, argc, argv);
        mem_free(input);
        mem_free(argv);
    }
}