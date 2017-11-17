#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "envlist.h"

NODE_LIST* initEnvironList(void) {
    return NULL;
}

NODE_LIST* setEnviron(NODE_LIST *FirstNode, ENV_VAR dataEnviron) {
    NODE_LIST *newElement; // Create a new node, which contains the id and the value of the new environment variable
    NODE_LIST *auxiliary = NULL; // Pointer to the previous list's element
    NODE_LIST *ptaux = FirstNode; // Update the first element with an auxiliary pointer for it
    newElement = (NODE_LIST*) malloc(sizeof(NODE_LIST)); // Allocate memory for the new list's node
    newElement->data = dataEnviron; // Insert data into the new element
    int i = 0; // Counter of shell's environment variable
    if(FirstNode == NULL) {
        FirstNode = newElement; // Pointer, which points to the beginning, receives the new element, which also is gonna be put in the beginning of the list
        newElement->next = newElement; // New element is also the last element of the list
    }
    else {
        do {
            previous = ptaux; // Save the last position of the auxiliary pointer in the previous element
            ptaux = ptaux->next;
            i++;
        }
        while(ptaux != FirstNode);  // Stop condition for the used circular list
        // Close the circular chaining
        auxiliary->next = newElement;    //(*previous).next points to the next element
        newElement->next = FirstNode; //(*new).next points to the beginning the list
    }
    return FirstNode; // It always comes to the beginning of the list
}