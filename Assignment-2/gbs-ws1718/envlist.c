#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "envlist.h"

NODE_LIST* initEnvironList(void) {
    return NULL;
}

NODE_LIST* setUpShellEnv(NODE_LIST *FirstNode) {
    ENV_VAR shellInfo; // It contains the information of the shell executable
    char path[1024] = "";
    char shellPath[1024] = ""; // For the executable of the shell
    strcat(shellPath, getcwd(path, sizeof(path)));
    strcat(shellPath, "/gbsh");
    strcpy(shellInfo.envVarId, "shell");
    strcpy(shellInfo.envVarValue, shellPath);
    FirstNode = setEnviron(FirstNode, shellInfo);
    return FirstNode;
}

NODE_LIST* setEnviron(NODE_LIST *FirstNode, ENV_VAR dataEnviron) {
    NODE_LIST *newElement; // Create a new node, which contains the id and the value of the new environment variable
    NODE_LIST *previous = NULL; // Pointer to the previous list's element
    NODE_LIST *ptaux = FirstNode; // Update the first element with an auxiliary pointer for it
    newElement = (NODE_LIST*) malloc(sizeof(NODE_LIST)); // Allocate memory for the new list's node
    newElement->data = dataEnviron; // Insert data into the new element
    int i = 0; // Counter of shell's environment variables
    int varSet = 0;
    if(FirstNode == NULL) {
        FirstNode = newElement; // Pointer, which points to the beginning, receives the new element, which also is gonna be put in the beginning of the list
        newElement->next = NULL; // New element is the last element of the linked list, so it points to null
    }
    else {
        do {
            if (strcmp(ptaux->data.envVarId, dataEnviron.envVarId) == 0) { // If the environment variable already exists
                strcpy(ptaux->data.envVarValue, dataEnviron.envVarValue);
                varSet = 1;
            }
            previous = ptaux; // Save the last position of the auxiliary pointer in the previous element
            ptaux = ptaux->next;
            i++;
            
        } while(ptaux != NULL);  // Stop condition for the used for this linked list
        
        if (varSet != 1) { // The variable hadn't been defined before
            previous->next = newElement;    //(*previous).next points to the next element
            newElement->next = NULL; //(*new).next points to the beginning the list
        }
        else {
            previous->next = NULL;
        }    
    }
    return FirstNode; // It always comes to the beginning of the list
}

NODE_LIST* unsetEnviron(NODE_LIST *FirstNode, char* varToBeDel) {
    if (FirstNode == NULL) {
        return NULL; // Empty list of environment variables
    }
    else if (strcmp(FirstNode->data.envVarId, varToBeDel) == 0) { // The element to be found is in the first position of the list
        NODE_LIST *tempNext;
        tempNext =  FirstNode->next; // Save the current next element, because it'll be the new first one in the list
        FirstNode->next = NULL; // Deallocate the First Node because the variable to be deleted was in it 
        return tempNext; // To have the new first element
    }
    /* Search the element in the rest of the list recursively */
    FirstNode->next = unsetEnviron(FirstNode->next, varToBeDel);
    return FirstNode;
}

int showEnvironList(NODE_LIST *FirstNode, char *output) {
    NODE_LIST *ptaux; // Aux which will go through the list
    int counter = 0; // Counter to define the number of set environment variables
    int len = 0;
    
    if(FirstNode == NULL)
        puts("\nThere's no defined environment variable");
    else {
        ptaux = FirstNode; // The aux points to the first node as well, in order to start going through all the nodes sequentially
        while(ptaux != NULL) {
            len += sprintf(output + len, "%s %s\n", ptaux->data.envVarId, ptaux->data.envVarValue); // Print "id value"
            ptaux = ptaux->next; // Go to the next list's element
            counter++; // For each node is a new variable
        }
    }
    return counter;
}