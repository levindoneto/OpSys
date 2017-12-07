#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *LOGFILE = "/tmp/gbsd.log";

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

int main(int argc, char *argv[]) {
    // fork off the parent process
    pid_t pid = fork();

    if (pid < 0) {
        printf("%s", strerror(errno));
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
        log_message(LOGFILE, "%s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // change directory to root
    if (chdir("/") < 0) {
        log_message(LOGFILE, "%s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // close standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // daemon
    pid_t dpid = getpid();
    int seconds = 0;
    while (1) {
        log_message(LOGFILE, "[STATUS] PID %d - %d", dpid, seconds);
        seconds++;
        sleep(1);
    }

    return 0;
}
