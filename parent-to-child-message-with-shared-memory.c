#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
    int shm_id = shmget(IPC_PRIVATE, 32, IPC_CREAT | 0666);

    int pid = fork();
    if (pid < 0) {
        // If the pid < 0, then an error has occured
        fprintf(stderr, "Fork failed.");
        return 1;
    } else if (pid == 0) {
        // If the pid == 0, then the process we're in is the child
        void *mem = shmat(shm_id, NULL, 0);
        memset(mem, 0, 32);
        sprintf(mem, "Hello World");
        shmdt(mem);
    } else {
        // If the pid > 0, then the process we're in is the parent
        waitpid(pid, NULL, 0);
        void *mem = shmat(shm_id, NULL, 0);
        printf("The message the parent recieved from the child is: %s.\n", (char*) mem);
        shmdt(mem);
        shmctl(shm_id, IPC_RMID, NULL);
    }
    return 0;
}
