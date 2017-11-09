/* Shell GBSH Development
 * Features: prompt, exit, pwd, sum-max
 * Members: Leonardo Nahra, Levindo Neto and Li-Wei Chen
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include "gbsh.h"
#include "summax.h"

#define EXIT "exit"
#define PWD "pwd"
#define SUMMAX "sum-max"
#define LS "ls"
#define MIN 1
#define MAX 5 // Matrices contain max 5 rows and 5 columns
#define MINELEMENT 0 // Minimum value for the random matrices
#define MAXELEMENT 100 // Maximum value for the random matrices
#define NORMAL_COLOR  "\x1B[0m" // White
#define DIRCOLOR  "\x1B[32m" // Green
#define FILECOLOR  "\x1B[34m" // Blue

void prompt(UserInfo userInformation, char **command, char **directory) {
    int matrix_size;
    int **inputA;
    int **inputB;
    int **matrixD;
    int **matrixC; // Output
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
        if((*command)[i] == '\n') { // Command without directory
            (*command)[i] = '\0';
            *directory = (char*)malloc(1); // For not getting problems with null directories
            break;
        }
        else if ((*command)[i] == ' ') { // Command with dictionary
            for(i = 0; 1; i++) {
                if(i) {
                    *directory = (char*)realloc((*directory),i+1);
                }
                else {
                    *directory = (char*)malloc(i+1);
                }
                (*directory)[i] = getchar();
                if((*directory)[i] == '\n') {
                   (*directory)[i] = '\0';
                    break;
                }
            }
        
        }
    }

    printf("command: %s\n", *command);
    printf("directory: %s\n", *directory);
    
    if (strcmp("op", *directory) == 0) {
        printf("directory is op lol\n");
    }
    else if (strcmp(PWD, *command) == 0) {
        pwd();
    }
    else if (strcmp(SUMMAX, *command) == 0) {
        matrix_size = generateRandomValue (MIN, MAX);
        inputA = createRandomMatrix(matrix_size, MINELEMENT, MAXELEMENT);
        inputB = createRandomMatrix(matrix_size, MINELEMENT, MAXELEMENT);
        matrixD = createMaxMatrix(inputA, inputB, matrix_size);
        matrixC = createSumMatrix(matrixD, matrix_size);

        /* Tests with prints */
        printf("\nMatrix A (Input)\n");
        printMatrix(inputA, matrix_size, matrix_size); // Square matrix
        printf("\nMatrix B (Input)\n");
        printMatrix(inputA, matrix_size, matrix_size); // Square matrix
        printf("\nMatrix D (Auxiliar - Max values)\n");
        printMatrix(matrixD, matrix_size, matrix_size); // Square matrix
        printf("\nMatrix C (Output - Sum of the max values)\n");
        printMatrix(matrixC, 1, matrix_size); // Matrix with just 1 row

        freeMatrix(inputA, matrix_size);
        freeMatrix(inputB, matrix_size);
        freeMatrix(matrixD, matrix_size);
        freeOutput(matrixC, matrix_size);
    }
    else if (strcmp(LS, *command) == 0) {
        printf("ls...");
    }
    else if (strcmp(EXIT, *command) == 0) {
        exit(0);
    }
    else {
        printf("Unrecognized command");
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
    char *directory = NULL;
    UserInfo userInformation; /* It doesn't change during the prompt's execution
                               * user, host, cwd */
    storeInfo(&userInformation);
    do {
        prompt(userInformation, &command, &directory);
        free(command);
        free(directory);
    } while(strcmp(EXIT,command) != 0); // keep running unless the user types "exit"
    

    return 0; // Exit the shell
}
