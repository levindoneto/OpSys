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
    for (row = 0; row < size; row++) {
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

void freeOutput(int **matrix, int size) {
    int row;
    for (row = 0; row < 1; row++) {
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
    /* Fill the matrix D with the max elements from the input matrices*/
    for (r = 0; r < size; r++) {
        for (c = 0; c < size; c++) {
            matrixD[r][c] = maxElement(matrixA[r][c], matrixB[r][c]);
        }
    }
    return matrixD;
}

int **createSumMatrix (int **maxMatrix, int size) {
    /* Allocate aux matrix C for getting the sum of the max elements */
    int **matrixC = (int **)malloc(size * sizeof(int *));
    int r, c;
    int aux_sum;
    for (r = 0; r < 1; r++) { // Just one row for the output matrix
        matrixC[r] = (int *)malloc(size * sizeof(int));
    }

    /* Calculate the sum of the values from each column from the matrix passed
     * as parameter */
     for (c = 0; c < size; c++) { // Each matrix d's column
         aux_sum = 0;
         for (r = 0; r < size; r++) { // Each matrix d's row
             aux_sum += maxMatrix[r][c]; // element in column:c, row:r
         }
         matrixC[0][c] = aux_sum; // element in row:0, column:c in the output
     }
     return matrixC;
}

