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

static const char *LOGFILE = "/tmp/ipcd.log";

void log_message(const char *filename, const char* format, ...) {
    FILE *logfile;
    logfile = fopen(filename, "a");

    if (!logfile) {
        return;
    }

    va_list list;
    va_start( list, format );
    vfprintf(logfile, format, list);
    va_end( list );
    fprintf(logfile, "\n");
    fclose(logfile);
}

long long int fib(int n) {
    long long int a = 0;
    long long int b = 1;
    long long int c;

    if ( n == 0) {
        return 0;
    }

    int i;
    for (i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }

    return b;
}

int main(int argc, char *argv[]) {
    // fork off the parent process
    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // change the file mode mask;
    umask(0);

    // create new SID
    pid_t sid = setsid();

    if (sid < 0) {
        log_message(LOGFILE, "Create new SID: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // change directory to root
    if (chdir("/") < 0) {
        log_message(LOGFILE, "Change directory: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // close standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // open shared memory file
    int shm_fd = shm_open(SHARED_MEMORY, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (shm_fd < 0) {
        log_message(LOGFILE, "Open shared memory: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // truncate to shared memory struct size
    if (ftruncate(shm_fd, sizeof(struct shared_memory)) < 0) {
        log_message(LOGFILE, "Truncate shared memory: %s", strerror(errno));
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
        log_message(LOGFILE, "Map shared memory: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    shm->n = 0;
    shm->fibn = 0;

    // open semaphores
    sem_t *sem_in;
    sem_in = sem_open("/fibonacciin", O_CREAT, S_IRUSR | S_IWUSR, 0);

    if (sem_in == SEM_FAILED) {
        log_message(LOGFILE, "Open in semaphore: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    sem_t *sem_out;
    sem_out = sem_open("/fibonacciout", O_CREAT, S_IRUSR | S_IWUSR, 0);

    if (sem_out == SEM_FAILED) {
        log_message(LOGFILE, "Open out semaphore: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // daemon
    while (1) {
        // wait for client to input
        log_message(LOGFILE, "Wait");
        if (sem_wait(sem_in) < 0) {
            log_message(LOGFILE, "Wait semaphore: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }

        shm->fibn = fib(shm->n);
        log_message(LOGFILE, "Fibonacci %lld", shm->fibn);

        log_message(LOGFILE, "Post");
        if (sem_post(sem_out) < 0) {
            log_message(LOGFILE, "Post semaphore: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
