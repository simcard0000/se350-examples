#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <semaphore.h>
#include <stdbool.h>
#include <memory.h>
#include <pthread.h>

// When building, you must link with the external pthread library: for example, 'gcc linked-list-integrity.c -lpthread'

typedef struct single_node {
    void *element;
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

bool push_front(single_list_t *list, void *obj) {
    single_node_t *tmp = malloc(sizeof(single_node_t));
    if (tmp == NULL) {
        return false;
    }
    tmp->element = obj;
    sem_wait(&(list->sem));
    tmp->next = list->head;
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
    single_node_t *newHead = list->head->next;
    sem_wait(&(list->sem));
    free(list->head);
    list->head = newHead;
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
    int value1 = 0;
    int value2 = 5;
    int value3 = 7;
    push_front(&data_list, &value1);
    push_front(&data_list, &value2);
    push_front(&data_list, &value3);
}

void* task2(void *param) {
    int value4 = 9;
    int value5 = 34;
    int value6 = 222;
    pop_front(&data_list);
    push_front(&data_list, &value4);
    push_front(&data_list, &value5);
    pop_front(&data_list);
    push_front(&data_list, &value6);
}

void* task3(void *param) {
    pop_front(&data_list);
    pop_front(&data_list);
    pop_front(&data_list);
}

int main(int argc, char** argv) {
    // Initializing the shared linked list:
    single_list_init(&data_list);

    // Creating three threads that will run different tasks
    pthread_t tid[3];
    pthread_create(&tid[0], NULL, task1, NULL);
    pthread_create(&tid[1], NULL, task2, NULL);
    pthread_create(&tid[2], NULL, task3, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);

    //End result of the shared linked list:
    single_node_t *it = data_list.head;
    while (it != NULL) {
        printf("element: %d", *(int*)(it->element));
        it = it->next;
    }
    pthread_exit(0);
}