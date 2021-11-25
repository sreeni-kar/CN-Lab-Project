#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


/*
    Function declarations of all commands
*/
int ushell_grep(char **args);
int ushell_chmod(char **args);
int ushell_echo(char **args);
int ushell_cd(char **args);
int ushell_ps(char **args);
int ushell_help(char **args);
int ushell_exit(char **args);



char *ushell_list_commands[] = {
    "grep",
    "chmod",
    "echo",
    "cd",
    "ps",
    "exit",
    "help"
};

int (*ushell_func_commands[])(char **) = {
    &ushell_grep,
    &ushell_chmod,
    &ushell_echo,
    &ushell_cd,
    &ushell_ps,
    &ushell_exit,
    &ushell_help
};

int ushell_num_commands() {
    return sizeof(ushell_list_commands) / sizeof(char *);
}

// Function where the system command is executed
void execArgs(char** args)
{
	// Forking a child
	pid_t pid = fork();

	if (pid == -1) {
		printf("\nFailed forking child..");
		return;
	} else if (pid == 0) {
		if (execvp(args[0], args) < 0) {
			printf("\nCould not execute command..");
		}
		exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return;
	}
}

// All the seven function of Seven commands go here
//exit is already done


















int ushell_exit(char **args) {
    return 0;
}

/*
    This fucntion will check the given input with all the available
    functions and executes them
    Input - array of tokens generated from ushell_splitline
    Output - Integer (return 0 only when the command is exit).
*/
int ushell_execute(char **args) {
    int i;
    
    //Empty argument
    if (args[0] == NULL) {
        return 1;
    }

    for (i = 0; i < ushell_num_commands(); i++){
        if (strcmp(args[0], ushell_list_commands[i]) == 0) {
            return (*ushell_func_commands[i])(args);
        }
    }

    return execArgs(args);

}


#define USHELL_TOK_DELIM " \t\r\n\a"

/*
    parameters - A variable line of type char *
    return - array of tokens from the input line

    This function splits the line into array of tokens at " \t\r\n\a"
*/
char **ushell_splitline(char *line){
    int bufsize = 64, position = 0;
    char **tokens = malloac(bufsize * sizeof(char *));
    char *token;

    if(!tokens) {
        fprintf(stderr, "ushell: Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, USHELL_TOK_DELIM);
    while(token != NULL) {
        tokens[position] = token;
        position++;


        if (position >= bufsize) {
            bufsize += 64;
            tokens = realloc(token, bufsize *sizeof(char *));
            if(!tokens) {
                fprintf(stderr, "ushell: Memory allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, USHELL_TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}




char* ushell_readline(){
    char *line = NULL;
    ssize_t bufsize = 0;

    if (getline(&line, &bufsize, stdin) == -1) {
        if(feof(stdin)){ 
            //When user types ctrl+D EOF occurs and shell exit with EXIT_SUCCESS
            exit(EXIT_SUCCESS);
        }else{
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}




void ushell_loop(){
    char *line;
    char **args;
    int status;

    do{
        //We can add pwd to this after pwd function is done.
        printf(">> ");
        line = ushell_readline();
        args = ushell_splitline(line);
        status = ushell_execute(args);

        free(line);
        free(args);
    }while(status);
}


int main(int argc, char** argv){
    
    ushell_loop();

    return EXIT_SUCCESS;
}
