#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stdio.h"
#include "summax.h"

int random_int(int min, int max) {
    return min + rand() % (max + 1 - min);
}

int** create_matrix(int m, int k) {
    int **matrix = malloc(m * sizeof *matrix);

    int i;
    for (i = 0; i < m; i++) {
        matrix[i] = malloc(k * sizeof **matrix);
    }

    int j;
    for (i = 0; i < m; i++) {
        for (j = 0; j < k; j++) {
            matrix[i][j] = random_int(0, 10);
        }
    }

    return matrix;
}

void free_matrix(int **matrix, int m) {
    int i;
    for (i = 0; i < m; i++) {
        free(matrix[i]);
    }

    free(matrix);
}

void print_matrix(int **matrix, int m, int k) {
    int i;
    int j;
    for (i=0; i < m; i++) {
        for (j = 0; j < k; j++){
            printf(" %2d", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    srand(time(0));

    int m = random_int(1, 5);
    int k = random_int(1, 5);
    int **A = create_matrix(m, k);
    int **B = create_matrix(m, k);
    int *C = malloc(k * sizeof *C);

    printf("Matrix A\n");
    print_matrix(A, m, k);

    printf("Matrix B\n");
    print_matrix(B, m, k);

    summax(A, B, m, k, C);

    printf("Vector C\n");
    print_matrix(&C, 1, k);

    free_matrix(A, m);
    free_matrix(B, m);
    free(C);

    return 0;
}
