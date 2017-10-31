#include <stdio.h>
#include <stdlib.h>
#include "summax.h"

int **summax (int size, int **inputA, int **inputB) {
    //ToDo
    printf("...");
    return 0;
}

int generateRandomValue (int min, int max) {
    return min + rand() % (max+1 - min);
}

int **createRandomMatrix (int size, int min_element_value, int max_element_value) {
    int **randomMatrix = (int **)malloc(size * sizeof(int *)); // Allocate the rows
    int row, column;
    for (row = 0; row < size; row++) { // Allocate the rows
        randomMatrix[row] = (int *)malloc(size * sizeof(int));
    }
    
    /* Fill matrix with random values */
    for (row=0; row<size; row++) {
        for (column=0; column<size; column++){
            randomMatrix[row][column] = generateRandomValue(min_element_value, max_element_value);
        }
    }
    return randomMatrix;
}

void freeMatrix(int **matrix, int size) {
    int row;
    for (row = 0; row < size; row++) {
         free(matrix[row]); // Free row
    }
    free(matrix); // Free rown's pointers
}

int maxElement (int elementA, int elementB) {
    int biggest = elementA;
    if (elementA < elementB) {
        biggest = elementB;
    }
    return biggest;
}

int **createMaxMatrix (int **matrixA, int **matrixB, int size) {
    int c, r;
    /* Allocate aux matrix D for getting the max elements */
    int **matrixD = (int **)malloc(size * sizeof(int *));
    for (r = 0; r < size; r++) {
        matrixD[r] = (int *)malloc(size * sizeof(int));
    }
    /* Filling the matrix D with the max elements from the input matrices*/
    for (r = 0; r < size; r++) {
        for (c = 0; c < size; c++) {
            matrixD[r][c] = maxElement(matrixA[r][c], matrixB[r][c]);
        }
    }
    return matrixD;
}


