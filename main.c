#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_LEN 256
#define ERROR_CODE -1
#define SUCCESS_CODE 0
/* text to be printed */
#define TEXT "aaa"

void exitWithFailure(const char *msg, int errcode){
    errno = errcode;
    fprintf(stderr, "%.256s:%.256s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}

void *routine(void *data){
    while (1){
        /* 
            explicit cancellation point
            (since Solaris printf doesn`t have
            implicit one)
        */
        pthread_testcancel();
        printf("%.256s\n", TEXT);
    }

    return SUCCESS_CODE;
}

int main(int argc, char **argv){
    pthread_t pid;

    int err = pthread_create(&pid, NULL, routine, NULL);
    if (err == ERROR_CODE)
        exitWithFailure("main", errno);

    sleep(2);
    err = pthread_cancel(pid);
    if (err == ERROR_CODE)
        exitWithFailure("main", errno);

    /*
        calling pthread_exit not to terminate
        thread before cancellation
    */
    pthread_exit((void*)SUCCESS_CODE);
}
