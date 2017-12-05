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
#include <fcntl.h>
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
#define ENVIRON "environ" // List all the environment variables which have been already defined
#define SETENV "setenv"
#define UNSETENV "unsetenv"
#define MIN 1
#define MAX 5 // Matrices contain max 5 rows and 5 columns
#define MINELEMENT 0 // Minimum value for the random matrices
#define MAXELEMENT 100 // Maximum value for the random matrices
#define NORMAL_COLOR  "\x1B[0m" // White
#define DIRCOLOR  "\x1B[32m" // Green
#define FILECOLOR  "\x1B[34m" // Blue
#define NULLSTR ""

NODE_LIST* firstElement; // Pointer for the first element of the list

void prompt() {
    UserInfo userInformation;
    char cmd[1024];
    int input;
    int output;
    char input_path[1024] = "";
    char output_path[1024] = "";
    int cmdc = 0;
    struct command cmdv[256];
    int status;
    bool background = false;
    while (true) {
        // break if executed background command
        if (background) {
            break;
        }

        // check for any child process that finish
        waitpid(-1, &status, WNOHANG);

        storeInfo(&userInformation);
        printf("\n%s%s@%s: %s > ", NORMAL_COLOR, userInformation.user, userInformation.host, userInformation.cwd);
        fgets(cmd, sizeof(cmd), stdin);
        parse_cmd(cmd, &cmdc, cmdv, input_path, output_path, &background);

        if (cmdc == 0) {
            continue;
        }

        // break on exit command
        if (strcmp(EXIT, cmdv[0].argv[0]) == 0) {
            break;
        }

        // fork on background execution
        if (background) {
            int pid = fork();
            if (pid == 0) {
                setpgid(0, 0);
            }
            else {
                background = false;
                free_cmd(cmdc, cmdv);
                cmdc = 0;
                continue;
            }
        }

        input = STDIN_FILENO;
        output = STDOUT_FILENO;

        // open input and output files if given
        if (strcmp(input_path, "") != 0) {
            input = open(input_path, O_RDONLY);
        }

        if (strcmp(output_path, "") != 0) {
            output = open(output_path, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
        }

        exec_commands(input, output, cmdc, cmdv);
        free_cmd(cmdc, cmdv);
        cmdc = 0;

        // close input and output files if given
        if (input != STDIN_FILENO) {
            close(input);
            strcpy(input_path, "");
        }

        if (output != STDOUT_FILENO) {
            close(output);
            strcpy(output_path, "");
        }
    }

    free(firstElement);
}

void parse_cmd(char *cmd, int *cmdc, struct command *cmdv, char *input_path, char *output_path, bool *background) {
    int cmd_count = 0;
    char *arg = strtok(cmd, " \n");
    char args[256][256];

    if (arg == 0) {
        return;
    }

    int i = 0;
    while (arg != 0) {
        // look for input redirection
        if (strcmp(arg, "<") == 0)  {
            arg = strtok(0, " \n");
            if (arg != 0) {
                strcpy(input_path, arg);
            }
        }
        // look for output redirection
        else if (strcmp(arg, ">") == 0)  {
            arg = strtok(0, " \n");
            if (arg != 0) {
                strcpy(output_path, arg);
            }
        }
        // look for background execution
        else if (strcmp(arg, "&") == 0) {
            *background = true;
        }
        // look for command piping
        else if (strcmp(arg, "|") == 0) {
            int argc = i;
            cmdv[cmd_count].argv = malloc((argc + 1) * sizeof(char*));
            cmdv[cmd_count].argv[argc] = malloc(1);
            cmdv[cmd_count].argv[argc] = 0;

            for (i = 0; i < argc; i++) {
                cmdv[cmd_count].argv[i] = malloc(strlen(args[i]) + 1);
                strcpy(cmdv[cmd_count].argv[i], args[i]);
            }

            cmd_count += 1;
            i = 0;
        }
        else {
            strcpy(args[i], arg);
            i++;
        }

        arg = strtok(0, " \n");
    }

    int argc = i;

    // alloc memory for the array, the array must be null terminated, so
    // we need an extra space in the array
    cmdv[cmd_count].argv = malloc((argc + 1) * sizeof(char*));

    // alloc and set null pointer at the end of the array
    cmdv[cmd_count].argv[argc] = malloc(1);
    cmdv[cmd_count].argv[argc] = 0;

    // copy the arguments from the temporary array to the output
    for (i = 0; i < argc; i++) {
        cmdv[cmd_count].argv[i] = malloc(strlen(args[i]) + 1);
        strcpy(cmdv[cmd_count].argv[i], args[i]);
    }

    cmd_count += 1;
    *cmdc = cmd_count;
}

void free_cmd(int cmdc, struct command *cmdv) {
    int argc;

    int i;
    for (i = 0; i < cmdc; i++) {
        argc = 0;
        while (cmdv[i].argv[argc] != 0) {
            free(cmdv[i].argv[argc]);
            argc++;
        }
        free(cmdv[i].argv[argc]);
        free(cmdv[i].argv);
    }
}

int spawn_proc(int in, int out, char **argv) {
    int pid = fork();

    if (pid == 0) {
        if (in != STDIN_FILENO) {
            dup2(in, STDIN_FILENO);
            close(in);
        }

        if (out != STDOUT_FILENO) {
            dup2(out, STDOUT_FILENO);
            close(out);
        }

        if (strcmp(PWD, argv[0]) == 0) {
            char path[1024] = "";

            if (getcwd(path, sizeof(path)) == 0) {
                printf("\n%s", strerror(errno));
            }
            else {
                printf("%s", path);
            }
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
            char input[1024] = "";

            if (in != STDIN_FILENO) {
                read(STDIN_FILENO, input, sizeof(input));
                strcpy(input, strtok(input, "\n"));
            }
            else if (argv[1] != 0) {
                strcpy(input, argv[1]);
            }

            char output[2048];
            ls(input, output);
            printf("%s", output);
        }
        else if (strcmp(CD, argv[0]) == 0) {
            if (argv[1] != 0) {
                cd(argv[1]);
            }
            else {
                cd("");
            }
        }
        else if (strcmp(ENVIRON, argv[0]) == 0) { // List set environment variables
            char output[2048];
            showEnvironList(firstElement, output);
            printf("%s", output);
        }
        else if (strcmp(SETENV, argv[0]) == 0) { // List set environment variables
            if (argv[1] != 0) {
                ENV_VAR newEnvVar; // id and value
                if (argv[2] == 0) { // If the user hasn't given a value for the new env var
                    strcpy(newEnvVar.envVarValue, "");
                }
                else {
                    strcpy(newEnvVar.envVarValue, argv[2]);
                }
                strcpy(newEnvVar.envVarId, argv[1]);
                firstElement = setEnviron(firstElement, newEnvVar);
            }
            else {
                printf("\nsetenv needs two parameters:\nid and value\nin order to function properly\n");
            }
        }
        else if (strcmp(UNSETENV, argv[0]) == 0) { // List set environment variables
            if (argv[1] != 0) {
                firstElement = unsetEnviron(firstElement, argv[1]);
            }
            else {
                printf("\nunsetenv needs one parameter:\nid of the environment variable\nin order to function properly\n");
            }
        }
        else {
            return execvp(argv[0], argv);
        }

        exit(0);
    }

    return pid;
}

void exec_commands(int input, int output, int n, struct command *cmd) {
    int pid;
    int status;
    int in = input;
    int fd[2];

    int i;
    for (i = 0; i < n - 1; i++) {
        // open pipe for command communication
        // process writes to write end of pipe
        // next process reads from read end of pipe
        pipe(fd);
        pid = spawn_proc(in, fd[1], cmd[i].argv);
        waitpid(pid, &status, 0);
        close(fd[1]);

        if (in != STDIN_FILENO) {
            close(in);
        }

        in = fd[0];
    }

    // last process reads from read end of last pipe
    // and writes to output file or stdout
    pid = spawn_proc(in, output, cmd[n - 1].argv);
    waitpid(pid, &status, 0);
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

void ls(char *folder, char *output) {
    if (strcmp(NULLSTR, folder) == 0) {
        folder = ".";
    }

    DIR * dir = opendir(folder);

    if (dir) {
        struct dirent *entry;

        int len = 0;
        while ((entry = readdir(dir)) != 0) {
            // If entry is a file
            if (entry->d_type != DT_DIR) {
                len += sprintf(output + len, "%s%s\n", FILECOLOR, entry->d_name);

            // If entry is a directory
            }
            else if (entry->d_type == DT_DIR
                       && strcmp(entry->d_name,".") != 0
                       && strcmp(entry->d_name,"..") != 0 ) {
                len += sprintf(output + len, "%s%s/\n", DIRCOLOR, entry->d_name);
            }
        }

        closedir(dir);
    } else {
        printf("%s", strerror(errno));
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
    firstElement = initEnvironList(); // Initialize the simple linked list of environment variables
    firstElement = setUpShellEnv(firstElement); // Add shell = <pathname>/gbsh as the first env var
    prompt();
    return 0; // Exit the shell
}
