#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    pid_t pid;
    int child_status;
    // First, let's fork a child process (parent spawns the child):
    pid = fork();
    // Next, let's define what the parent and child processes should do:
    if (pid < 0) {
        // If the pid < 0, then an error has occured
        fprintf(stderr, "Forking a child process has failed.");
    } else if (pid == 0) {
        // If the pid == 0, then the process we're in is the child
        execlp("/bin/ls", "ls", NULL);
    } else {
        // If the pid > 0, then the process we're in is the parent
        wait(&child_status);
        printf("The child process has completed with status: %i \n", child_status);
    }
    return 0;
    // As we don't know what process is scheduled to run first, we need to account for this in the code.
}