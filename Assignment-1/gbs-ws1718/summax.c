#include <stdio.h>
#include <stdlib.h>
#include "summax.h"

int **summax (int size, int **inputA, int **inputB) {
    //ToDo
    printf("...");
    return 0;
}

int generateRandomSize (int min, int max) {
    return min + rand() % (max+1 - min);
}

int **createRandomMatrix (int size) {
	// allocate Rows rows, each row is a pointer to int
    int **randomMatrix = (int **)malloc(size * sizeof(int *)); // Allocate the rows
    int row;
    for (row = 0; row < size; row++) { // Allocate the rows
        randomMatrix[row] = (int *)malloc(size * sizeof(int));
    }
    return randomMatrix;
}

void freeMatrix(int **matrix, int size) 
{
    int row;
    for (row = 0; row < size; row++) {
         free(matrix[row]); // Free row
    }
    free(matrix); // Free rown's pointers
 }