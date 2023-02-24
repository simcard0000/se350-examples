#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int sum;

void *runner(void *param) {
    int upper = atoi(param);
    sum = 0;
    for (int i = 1; i <= upper; i++) {
        sum += i;
    }
    pthread_exit(0);
}

int main(int argc, char** argv) {
    pthread_t tid; // the thread identifier
    pthread_attr_t attr; // the set of thread attributes

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <integer value>\n", argv[0]);
        return -1;
    }
    // setting the default attributes:
    pthread_attr_init(&attr);
    // creating the thread:
    pthread_create(&tid, &attr, runner, argv[1]);
    pthread_join(tid, NULL);
    printf("sum = %d\n", sum);
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
}