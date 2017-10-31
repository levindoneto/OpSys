/* Shell GBSH Development
 * Features: prompt, exit, pwd, sum-max
 * Members: Leonardo Nahra, Levindo Neto and Li Smile
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "gbsh.h"
#include "summax.h"

#define EXIT "exit"
#define PWD "pwd"
#define SUMMAX "sum-max"
#define MIN 1
#define MAX 5 // Matrices with max 5 rows and 5 columns

void prompt(UserInfo userInformation, char **command) {
    int matrix_size;
    printf("\n<%s>@<%s> <%s> > ", userInformation.user, userInformation.host, userInformation.cwd);
    int i;
    for(i = 0; 1; i++) {
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
    if (strcmp(PWD, *command) == 0) {
        pwd();
    }
    else if (strcmp(SUMMAX, *command) == 0) {
        matrix_size = generateRandomSize (MIN, MAX);
    }
    else if (strcmp(EXIT, *command) == 0) {
        exit(0);
    }

    else {
        printf("\nUnrecognized command");
    }
}

void storeInfo (UserInfo *infoUser) {
    /* CWD: Current path */
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
       infoUser->cwd = "cwd";
    }
    else {
        infoUser->cwd = cwd;
    }

    /* Logged username */
    char *username = getenv("USER");
    if (username==NULL) {
        infoUser->user = "user";
    }
    else {
        infoUser->user = username;
    }
    /* Logged Host */
    char hostname[1024];
    gethostname(hostname, 1024);
    infoUser->host = hostname;
}

void pwd () {
    char currentpath[1024];
    if (getcwd(currentpath, sizeof(currentpath)) != NULL) {
        printf("%s", currentpath);
    }
    else {
        printf("Current path");
    }
}

int main(int argc, char *argv[]) {
    char *command = NULL;
    UserInfo userInformation; /* It doesn't change during the prompt's execution
                               * user, host, cwd */
    storeInfo(&userInformation);
    do {
        prompt(userInformation, &command);
    } while(strcmp(EXIT,command) != 0); // keep running unless the user types "exit"
    free(command);

    return 0; // Exit the shell
}
