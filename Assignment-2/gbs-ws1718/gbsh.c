/* Shell GBSH Development
 * Features: prompt, exit, pwd, sum-max
 * Members: Leonardo Nahra, Levindo Neto and Li-Wei Chen
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include "gbsh.h"
#include "summax.h"
#include "envlist.h"

#define EXIT "exit"
#define PWD "pwd"
#define SUMMAX "sum-max"
#define LS "ls"
#define CD "cd"
#define MIN 1
#define MAX 5 // Matrices contain max 5 rows and 5 columns
#define MINELEMENT 0 // Minimum value for the random matrices
#define MAXELEMENT 100 // Maximum value for the random matrices
#define NORMAL_COLOR  "\x1B[0m" // White
#define DIRCOLOR  "\x1B[32m" // Green
#define FILECOLOR  "\x1B[34m" // Blue
#define NULLSTR ""
#define TESTENVLIST 0 // Change to 0 to execute the shell normally

void prompt() {
    UserInfo userInformation;

    char cmd[1024];
    int argc = 0;
    char **argv;
    while (true) {
        storeInfo(&userInformation);
        printf("\n%s@%s: %s $ ", userInformation.user, userInformation.host, userInformation.cwd);
        fgets(cmd, sizeof(cmd), stdin);
        parse_cmd(cmd, &argc, &argv);

        if (argc == 0) {
            continue;
        }

        if (strcmp(PWD, argv[0]) == 0) {
            pwd();
        }
        else if (strcmp(SUMMAX, argv[0]) == 0) {
            int matrix_size;
            int **inputA;
            int **inputB;
            int **matrixD;
            int **matrixC; // Output

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
        else if (strcmp(LS, argv[0]) == 0) {
            if (argc > 1) {
                ls(argv[1]);
            } else {
                ls("");
            }
        }
        else if (strcmp(CD, argv[0]) == 0) {
            if (argc > 1) {
                cd(argv[1]);
            } else {
                cd("");
            }
        }
        else if (strcmp(EXIT, argv[0]) == 0) {
            break;
        }
        else {
            int pid = fork();

            if (pid == 0) {
                execvp(argv[0], argv);
                free_cmd(argc, &argv);
                argv = 0;
                exit(0);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
        }

        free_cmd(argc, &argv);
        argv = 0;
    }
}

void parse_cmd(char *cmd, int *argc, char ***argv) {
    char *arg = strtok(cmd, " \n");
    char args[256][256];

    // parse command line string and store in a temporary array
    int i = 0;
    while (arg != 0) {
        strcpy(args[i], arg);
        i++;
        arg = strtok(0, " \n");
    }

    *argc = i;

    // alloc memory for the array, the array must be null terminated, so
    // we need an extra space in the array
    *argv = malloc((*argc + 1) * sizeof(char*));

    // alloc and set null pointer at the end of the array
    (*argv)[*argc] = malloc(1);
    (*argv)[*argc] = 0;

    // copy the arguments from the temporary array to the output
    for (i = 0; i < *argc; i++) {
        (*argv)[i] = malloc(strlen(args[i]) + 1);
        strcpy((*argv)[i], args[i]);
    }
}

void free_cmd(int argc, char ***argv) {
    int i;
    for (i = 0; i <= argc; i++) {
        free((*argv)[i]);
    }
    free(*argv);
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

void ls(char *folder) {
    if (strcmp(NULLSTR, folder) == 0) {
        folder = ".";
    }

    DIR * dir = opendir(folder);

    if (dir) {
        struct dirent *entry;

        while ((entry = readdir(dir)) != 0) {
            // if entry is a file
            if (entry->d_type != DT_DIR) {
                printf("%s%s\n%s", FILECOLOR, entry->d_name, NORMAL_COLOR);

            // if entry is a directory
            } else if (entry->d_type == DT_DIR
                       && strcmp(entry->d_name,".") != 0
                       && strcmp(entry->d_name,"..") != 0 ) {
                printf("%s%s/%s\n", DIRCOLOR, entry->d_name, NORMAL_COLOR);
            }
        }

        closedir(dir);
    } else {
        strerror(errno);
    }
}

void cd(char *folder) {
    DIR* dir = opendir(folder);
    if (dir) {
       /* If the user does not pass a folder as parameter, folder="", and
       chdir("") is the same as chdir("./") */
        chdir(folder);
    }
    else if (ENOENT == errno) {
        printf("The system cannot find the specified directory");
    }
    else {
        printf("\nThe system has found an error\n");
    }

}

int main(int argc, char *argv[]) {
    #if TESTENVLIST == 1
        ENV_VAR environmentVar;
        int amountEnvironVars = 0; // Number of environment variables which have been already set
        NODE_LIST* firstElement; // Pointer for the first element of the list
        int wish; // Yes(1) / No(2)

        firstElement = initEnvironList(); // Initialize the circular list of environment variables

        printf("\nEnviron\n\n");
        do {
            amountEnvironVars++;
            printf("\nId: ");
            scanf("%s", environmentVar.envVarId);
            printf("Value: ");
            scanf("%s", environmentVar.envVarValue);
            firstElement = setEnviron(firstElement, environmentVar);
            printf("\n1-Yes | 2-No): ");
            scanf("%d",&wish);
            fflush(stdin);
        }
        while(wish!=2);
        //---------------------------------------------------------------------
        printf("\nThere are %d set variables\n", showEnvironList(firstElement));
    #else
        prompt();
    #endif
    return 0; // Exit the shell
}
