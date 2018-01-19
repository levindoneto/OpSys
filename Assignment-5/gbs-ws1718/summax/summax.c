#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "summax.h"

struct input {
    int **A;
    int **B;
    int m;
    int k;
};

static void* run(void *args) {
    struct input in = *(struct input*)args;
    int sum = 0;

    int i;
    for (i = 0; i < in.m; i++) {
        if (in.A[i][in.k] >= in.B[i][in.k]) {
            sum += in.A[i][in.k];
        } else {
            sum += in.B[i][in.k];
        }
    }

    return (void*) sum;
}

int summax(int **A, int **B, int m, int k, int *C) {
    pthread_t *threads = malloc(k * sizeof *threads);
    struct input *args = malloc(k * sizeof *args);

    int j;
    for (j = 0; j < k; j++) {
        args[j] = (struct input) {
            .A = A,
            .B = B,
            .m = m,
            .k = j
        };

        if (pthread_create(&threads[j], 0, run, &args[j]) != 0) {
            perror("Create thread");
            exit(EXIT_FAILURE);
        }
    }

    for (j = 0; j < k; j++) {
        void *retval = 0;

        if (pthread_join(threads[j], &retval) != 0) {
            perror("Join thread");
            exit(EXIT_FAILURE);
        }

        C[j] = (int) retval;
    }

    return 0;
}
