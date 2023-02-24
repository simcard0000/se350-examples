#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_LEN 1024
pthread_rwlock_t rwlock;

void init() {
    pthread_rwlock_init(&rwlock, NULL);
}

void cleanup() {
    pthread_rwlock_destroy(&rwlock);
}

void write_data(FILE* f) {
    fprintf(f, "\nYAY IT WORKED\n");
}

void read_data(FILE* f) {
    char token[MAX_LEN + 1];
    while (fscanf(f, " %s", &token[0]) == 1) { // the space in front of %s is important
        printf("%s\n", token);
    }
}

void* writer(FILE* f) {
    pthread_rwlock_wrlock(&rwlock);
    write_data(f);
    pthread_rwlock_unlock(&rwlock);
    pthread_exit(NULL);
}

void* reader(FILE* f) {
    pthread_rwlock_rdlock(&rwlock);
    read_data(f);
    pthread_rwlock_unlock(&rwlock);
    pthread_exit(NULL);
}

int main(int argc, char** argv) {

    FILE* my_file = fopen(argv[1], "r+");
    if (my_file == NULL) {
        printf("Unable to open file! Is %s a valid name?", argv[1]);
        return -1;
    }

    pthread_t read;
    pthread_t write;
    init();
    pthread_create(&read, NULL, reader, my_file);
    pthread_create(&write, NULL, writer,  my_file);
    pthread_join(read, NULL);
    pthread_join(write, NULL);
    cleanup();

    fclose(my_file);
    pthread_exit(0);
}