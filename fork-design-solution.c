#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
    pid_t pid;
    int child_result;
    int parent_result;

    pid = fork();

    if (pid < 0) {
        return -1;
    } else if (pid == 0) {
        return execute_B();
    } else {
        parent_result = execute_A();
        wait(&child_result);
    }

    if (child_result == 0 && parent_result == 0) {
        printf("Child and parent processes completed successfully.");
        return 0;
    }
    if (child_result != 0) {
        printf("Error occurred in child process: %d.\n", child_result);
    }
    if (parent_result != 0) {
        printf("Error occurred in parent process: %d.\n", parent_result);
    }
    return -1;
}

int executeA() {
    int value = execlp("/bin/ls", "ls", NULL);
    if (value != -1) {
        return 0;
    } else {
        return -1;
    }
}

int executeB() {
    int value = execlp("/bin/ls", "ls", NULL);
    if (value != -1) {
        return 0;
    } else {
        return -1;
    }
}