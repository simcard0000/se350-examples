#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define MY_FILE "<put the name of a .txt file here>"

int main(int argc, char** argv) {
    int fd = open(MY_FILE, O_RDWR);
    struct stat st;
    stat(MY_FILE, &st);
    ssize_t size = st.st_size;
    void *mapped = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    int pid = fork();
    if (pid < 0) {
        // If the pid < 0, then an error has occured
        fprintf(stderr, "Forking a child process has failed.");
        return 1;
    } else if (pid == 0) {
        // If the pid == 0, then the process we're in is the child
        memset(mapped, 0, size); // Erase the current contents of the file
        sprintf(mapped, "It is now overwritten.");
        // Ensure that data is synchronized:
        msync(mapped, size, MS_SYNC);
        munmap(mapped, size);
    } else {
        // If the pid > 0, then the process we're in is the parent
        waitpid(pid, NULL, 0);
        printf("The new content of the file is: %s.\n", (char*) mapped);
        munmap(mapped, size);
    }
    close(fd);
    return 0;
}