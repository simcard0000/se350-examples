#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <semaphore.h>
#include <stdbool.h>
#include <memory.h>
#include <pthread.h>

// When building, you must link with the external pthread library: for example, 'gcc linked-list-integrity.c -lpthread'

typedef struct single_node {
    int element;
    struct single_node *next;
} single_node_t;

typedef struct single_list {
    single_node_t *head;
    single_node_t *tail;
    int size;
    sem_t sem;
} single_list_t;

void single_list_init(single_list_t *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    sem_init(&(list->sem), 0, 1);
}

bool push_front(single_list_t *list, int obj) {
    single_node_t *tmp = malloc(sizeof(single_node_t));
    if (tmp == NULL) {
        return false;
    }
    tmp->element = obj;
    sem_wait(&(list->sem));
    tmp->next = list->head;
    list->head = tmp;
    if(list->size == 0) {
        list->tail = tmp;
    }
    ++(list->size);
    sem_post(&(list->sem));
    return true;
}

bool pop_front(single_list_t *list) {
    if (list->size == 0) {
        return false;
    }
    sem_wait(&(list->sem));
    single_node_t *oldHead = list->head;
    list->head = list->head->next;
    free(oldHead);
    --(list->size);
    if(list->size == 0) {
        list->tail = NULL;
    }
    sem_post(&(list->sem));
    return true;
}

single_list_t data_list;

// The below code is a personal example:

void* task1(void *param) {
    push_front(&data_list, 5);
    push_front(&data_list, 7);
    push_front(&data_list, 9);
    pthread_exit(0);
}

void* task2(void *param) {
    pop_front(&data_list);
    push_front(&data_list, 11);
    push_front(&data_list, 13);
    push_front(&data_list, 15);
    pop_front(&data_list);
    pthread_exit(0);
}

int main(int argc, char** argv) {
    // Initializing the shared linked list:
    single_list_init(&data_list);

    // Creating three threads that will run different tasks
    pthread_t tid[2];
    pthread_create(&tid[0], NULL, task1, NULL);
    pthread_create(&tid[1], NULL, task2, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    //End result of the shared linked list:
    single_node_t *it = data_list.head;
    while (it != NULL) {
        printf("element: %d\n", it->element);
        it = it->next;
    }
    pthread_exit(0);
}
