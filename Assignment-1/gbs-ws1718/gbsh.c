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

#define EXIT "exit"

void prompt(UserInfo userInformation, char **command) {
    printf("<%s>@<%s> <%s> > ", userInformation.user, userInformation.host, userInformation.cwd);
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
}

void storeInfo (UserInfo *infoUser) {
    /* CWD: Current path */
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    infoUser->cwd = cwd;
    /* Logged username */
    char *username = getenv("USER");
	if (username==NULL) {
		printf("User hasn't been found");
	}
	else {
		infoUser->user = username;
   	}
	/* Logged Host */
    //char hostname[1024];
    //gethostname(hostname, 1024);
    infoUser->host = "host"; //ToDo: Modify for unix implementation
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
