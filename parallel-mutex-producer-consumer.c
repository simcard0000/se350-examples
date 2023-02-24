#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <semaphore.h>

// When building, you must link with the external pthread library: for example, 'gcc parallel-mutex-producer-consumer.c -lpthread'
// This example involves 10 producers and 10 consumers.

#define BUFFER_SIZE 100
int buffer[BUFFER_SIZE];
int p_index = 0;
int c_index = 0;
sem_t spaces;
sem_t items;
pthread_mutex_t mutex;

int produce(int id) {
    int r = rand();
    printf("The producer of id: %d produced: %d.\n", id, r);
    return r;
}

void consume(int id, int value) {
    printf("The consumer of id: %d consumed: %d.\n", id, value);
    return;
}

void* producer(void* arg) {
    int* id = (int*) arg;
    for (int i = 0; i < 10000; i++) {
        int v = produce(*id);
        sem_wait(&spaces);
        pthread_mutex_lock(&mutex);
        buffer[p_index] = v;
        p_index = (p_index + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&items);
    }
    free(arg);
    pthread_exit(NULL);
}

void* consumer(void* arg) {
    int* id = (int*) arg;
    for (int i = 0; i < 10000; i++) {
        sem_wait(&items);
        pthread_mutex_lock(&mutex);
        int temp = buffer[c_index];
        buffer[c_index] = -1;
        c_index = (c_index + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&spaces);
        consume(*id, temp);
    }
    free(id);
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    sem_init(&spaces, 0, BUFFER_SIZE);
    sem_init(&items, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_t threads[20];
    for (int i = 0; i < 10; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, producer, id);
    }
    for (int i = 10; i < 20; i++) {
        int* id = malloc(sizeof(int));
        *id = i - 10;
        pthread_create(&threads[i], NULL, consumer, id);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    sem_destroy(&spaces);
    sem_destroy(&items);
    pthread_mutex_destroy(&mutex);
    pthread_exit(0);
}