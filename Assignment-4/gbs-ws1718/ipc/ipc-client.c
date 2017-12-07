#include "ipcd.h"
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // check argument validity
    if (argc != 2) {
        fprintf(stderr, "Invalid number of arguments: expected 2\n");
        exit(EXIT_FAILURE);
    }

    int n;
    if (sscanf(argv[1], "%d", &n) != 1) {
        fprintf(stderr, "Invalid argument type: expected integer\n");
        exit(EXIT_FAILURE);
    }

    if (n < 0) {
        fprintf(stderr, "Invalid argument: expected positive integer\n");
        exit(EXIT_FAILURE);
    }

    if (n > 92) {
        fprintf(stderr, "Invalid argument: must be equal or lower then 92\n");
        exit(EXIT_FAILURE);
    }

    // open shared memory file
    int shm_fd = shm_open(SHARED_MEMORY, O_RDWR, S_IRUSR | S_IWUSR);

    if (shm_fd < 0) {
        perror("Open shared memory");
        exit(EXIT_FAILURE);
    }

    // truncate to shared memory struct size
    if (ftruncate(shm_fd, sizeof(struct shared_memory)) < 0) {
        perror("Truncate shared memory");
        exit(EXIT_FAILURE);
    }

    // map shared memory
    struct shared_memory *shm;
    shm = mmap(
            NULL,
            sizeof(struct shared_memory),
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            shm_fd,
            0
            );

    if (shm == MAP_FAILED) {
        perror("Map shared memory");
        exit(EXIT_FAILURE);
    }

    // open semaphore
    sem_t *sem_in;
    sem_in = sem_open("/fibonacciin", 0);

    if (sem_in == SEM_FAILED) {
        perror("Open in semaphore");
        exit(EXIT_FAILURE);
    }

    sem_t *sem_out;
    sem_out = sem_open("/fibonacciout", 0);

    if (sem_out == SEM_FAILED) {
        perror("Open out semaphore");
        exit(EXIT_FAILURE);
    }

    shm->n = n;

    if (sem_post(sem_in) < 0) {
        perror("Semaphore post");
        exit(EXIT_FAILURE);
    }

    if (sem_wait(sem_out) < 0) {
        perror("Semaphore wait");
        exit(EXIT_FAILURE);
    }

    printf("Fibonacci %d = %lld\n", shm->n, shm->fibn);

    close(shm_fd);

    return 0;
}
