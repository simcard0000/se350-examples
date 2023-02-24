#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <semaphore.h>
#include <stdbool.h>
#include <memory.h>
#include <pthread.h>

// When building, you must link with the external pthread library: for example, 'gcc basic-search-insert-delete.c -lpthread'
// The linked list implementation is based on the code in linked-list-integrity.c

// Watch out for casting void* to int in an unsafe way: in that the values of sizeof(int) and sizeof(void*) might be different.
// You could suffer loss if the value that void* holds is beyond the maximum value for an int.

typedef struct single_node {
    int element;
    struct single_node *next;
} single_node_t;

typedef struct single_list {
    single_node_t *head;
    single_node_t *tail;
    int size;
} single_list_t;

void single_list_init(single_list_t *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

single_list_t data_list;

bool push_front(single_list_t *list, int obj) {
    single_node_t *tmp = malloc(sizeof(single_node_t));
    if (tmp == NULL) {
        return false;
    }
    tmp->element = obj;
    tmp->next = list->head;
    list->head = tmp;
    if(list->size == 0) {
        list->tail = tmp;
    }
    ++(list->size);
    return true;
}

bool pop_front(single_list_t *list) {
    if (list->size == 0) {
        return false;
    }
    single_node_t *oldHead = list->head;
    list->head = list->head->next;
    free(oldHead);
    --(list->size);
    if(list->size == 0) {
        list->tail = NULL;
    }
    return true;
}

bool find(int value) {
    single_node_t *it = data_list.head;
    bool found = false;
    while (it != NULL) {
        if (it->element == value) {
            found = true;
            break;
        }
        it = it->next;
    }
    return found;
}

// SEARCH-INSERT-DELETE functions:

pthread_mutex_t searcher_mutex;
pthread_mutex_t inserter_mutex;
pthread_mutex_t perform_insert_mutex;
sem_t no_searchers;
sem_t no_inserters;
int searchers;
int inserters;

void init() {
    pthread_mutex_init(&searcher_mutex, NULL);
    pthread_mutex_init(&inserter_mutex, NULL);
    pthread_mutex_init(&perform_insert_mutex, NULL);
    sem_init(&no_searchers, 0, 1);
    sem_init(&no_inserters, 0, 1);
    searchers = 0;
    inserters = 0;
}

void cleanup() {
    pthread_mutex_destroy(&searcher_mutex);
    pthread_mutex_destroy(&inserter_mutex);
    pthread_mutex_destroy(&perform_insert_mutex);
    sem_destroy(&no_searchers);
    sem_destroy(&no_inserters);
}

void* searcher(void* target) {
    pthread_mutex_lock(&searcher_mutex);
    searchers++;
    if (searchers == 1) {
        sem_wait(&no_searchers);
    }
    pthread_mutex_unlock(&searcher_mutex);
    bool result = find(*(int *)target);
    if (result) {
        printf("Found what you're looking for :)\n");
    } else {
        printf("It's not in the list :(\n");
    }
    pthread_mutex_lock(&searcher_mutex);
    searchers--;
    if (searchers == 0) {
        sem_post(&no_searchers);
    }
    pthread_mutex_unlock(&searcher_mutex);
    pthread_exit(NULL);
}

void* deleter(void* arg) {
    sem_wait(&no_searchers);
    sem_wait(&no_inserters);
    pop_front(&data_list);
    sem_post(&no_searchers);
    sem_post(&no_inserters);
    pthread_exit(NULL);
}

void* inserter(void* value) {
    pthread_mutex_lock(&inserter_mutex);
    inserters++;
    if (inserters == 1) {
        sem_wait(&no_inserters);
    }
    pthread_mutex_unlock(&inserter_mutex);
    pthread_mutex_lock(&perform_insert_mutex);
    push_front(&data_list, *(int *)value);
    pthread_mutex_unlock(&perform_insert_mutex);
    pthread_mutex_lock(&inserter_mutex);
    inserters--;
    if (inserters == 0) {
        sem_post(&no_inserters);
    }
    pthread_mutex_unlock(&inserter_mutex);
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    // Initializing the shared linked list and the semaphores/mutexes:
    single_list_init(&data_list);
    init();

    // Initializing four threads:
    pthread_t tid[4];
    int* value1 = malloc(sizeof(int));
    *value1 = 16;
    int* value2 = malloc(sizeof(int));
    *value2 = 24;
    pthread_create(&tid[0], NULL, inserter, value1);
    pthread_create(&tid[1], NULL, inserter, value2);
    pthread_create(&tid[2], NULL, deleter, NULL);
    pthread_create(&tid[3], NULL, searcher, value1);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);

    //End result of the shared linked list:
    single_node_t *it = data_list.head;
    single_node_t *old = NULL;
    while (it != NULL) {
        printf("element: %d\n", it->element);
        old = it;
        it = it->next;
        free(old);
    }
    cleanup();
    pthread_exit(0);
}
