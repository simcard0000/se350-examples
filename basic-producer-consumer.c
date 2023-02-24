#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 20;
sem_t spaces;
sem_t items;
int counter = 0;
int* buffer;

int produce() {
    ++counter;
    return counter;
}

void consume(int value) {
    printf("Consumed: %d.\n", value);
    return;
}

void* producer(void* arg) {
    int p_index = 0;
    while (counter < 10000) {
        int v = produce();
        sem_wait(&spaces);
        buffer[p_index] = v;
        p_index = (p_index + 1) % BUFFER_SIZE;
        sem_post(&items);
    }
    pthread_exit(NULL);
}

void* consumer(void* arg) {
    int c_index = 0;
    int c_total = 0;
    while (c_total < 10000) {
        sem_wait(&items);
        int temp = buffer[c_index];
        buffer[c_index] = -1;
        c_index = (c_index + 1) % BUFFER_SIZE;
        sem_post(&spaces);
        consume(temp);
        ++c_total;
    }
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    buffer = malloc(BUFFER_SIZE * sizeof(int));
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = -1;
    }
    sem_init(&spaces, 0, BUFFER_SIZE);
    sem_init(&items, 0, 0);

    pthread_t prod;
    pthread_t con;

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&con, NULL, consumer, NULL);
    pthread_join(prod, NULL);
    pthread_join(con, NULL);

    free(buffer);
    sem_destroy(&spaces);
    sem_destroy(&items);
    pthread_exit(0);
}