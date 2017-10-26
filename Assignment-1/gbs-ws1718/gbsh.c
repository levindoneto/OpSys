/* Shell GBSH Development
 * Features: prompt, exit, pwd, sum-max
 * Members: Leonardo Nahra, Levindo Neto and Li Smile
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gbsh.h"

#define EXIT "exit"

void prompt(char *statement, char **command) {
    printf("%s", statement);
    for(int i = 0; 1; i++) {
        if(i) {
            *command = (char*)realloc((*command),i+1);
        }
        else {
            *command = (char*)malloc(i+1);
        }
        (*command)[i] = getchar();
        if((*command)[i] == '\n') {
            (*command)[i] = '\0';
            break;
        }
    }
    //printf("%s", *command);
}

int main(int argc, char *argv[]) {
    char *command = NULL;
    do {
        prompt("<user>@<host> <cwd> > ", &command);
        free(command);
    } while(strcmp(EXIT,command) != 0); // keep running unless the user types "exit"
    free(command);
    printf("Out...");
    return 0; // Exit the shell
}
