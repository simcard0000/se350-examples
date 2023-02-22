#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/file.h> // needed for flock function

#define MAX_LEN 1024

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }
    // First, let's open the file for read and update:
    FILE* my_file = fopen(argv[1], "r+");
    if (my_file == NULL) {
        printf("Unable to open file! Is %s a valid name?", argv[1]);
        return -1;
    }
    // Next, let's place a file lock on the file we're trying to read and update to:
    int file_description = fileno(my_file);
    int lock_result = flock(file_description, LOCK_EX); // lockf or flock is library dependent
    printf("Result of locking the file: %d", lock_result);
    // Then, read the file by each string token (skipping whitespace, not using getline): 
    char token[MAX_LEN + 1];
    while (fscanf(my_file, " %s", &token[0]) == 1) { // the space in front of %s is important
        printf("%s\n", token);
    }
    // The pointer should be at the end of the file after reading, so let's add stuff now:
    fprintf(my_file, "\nYAY IT WORKED\n");
    // Let's remove the lock we put on the file:
    int unlock_result = flock(file_description, LOCK_UN);
    printf("Result of unlocking the file: %d", unlock_result);
    // And then close the file:
    fclose(my_file);
    // Lastly, uncomment to delete the file, otherwise keep it like this so that you can open the file and see what you added:
    // remove(argv[1]); 
    return 0;
}
