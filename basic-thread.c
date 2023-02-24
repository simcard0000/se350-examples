#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// When building, you must link with the external pthread library: for example, 'gcc curl.c -lpthread'

void* run(void* argument) {
    char* a = (char*) argument;
    printf("Provided argument: %s.\n", a);
    int* return_value = malloc(sizeof(int));
    *return_value = 99;
    pthread_exit(return_value);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Invalid arguments.\n");
        return -1;
    }
    pthread_t t;
    void* vr;

    pthread_create(&t, NULL, run, argv[1]);
    pthread_join(t, &vr);
    int* r = (int*) vr;
    printf("The other thread returned the value: %d.\n", *r);
    free(vr);
    pthread_exit(0);
}
