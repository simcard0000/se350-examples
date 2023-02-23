#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(int argc, char** argv) {
    char write_message[BUFFER_SIZE] = "Greetings";
    char read_message[BUFFER_SIZE];
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe failed.");
        return 1;
    }

    pid = fork();
    if (pid < 0) {
        // If the pid < 0, then an error has occured
        fprintf(stderr, "Fork failed.");
        return 1;
    } else if (pid == 0) {
        // If the pid == 0, then the process we're in is the child
        // First, close the unused end of the pipe:
        close(fd[WRITE_END]);
        // Then, read from the pipe:
        read(fd[READ_END], read_message, BUFFER_SIZE);
        printf("What the child read: %s", read_message);
        // Finally, close the read end of the pipe:
        close(fd[READ_END]);
    } else {
        // If the pid > 0, then the process we're in is the parent
        // First, close the unused end of the pipe:
        close(fd[READ_END]);
        // Then, write to the pipe:
        write(fd[WRITE_END], write_message, strlen(write_message));
        // Finally, close the write end of the pipe:
        close(fd[WRITE_END]);
    }
    return 0;
}