#include "ipcd.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void log_message(char *filename, const char* format, ...) {
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
    fprintf(stdout, "Hello World!\n");

    // IPC daemon code here

    // ...

    // ...

    return 0;
}
