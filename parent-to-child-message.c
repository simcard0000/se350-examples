#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>

struct message {
    long message_type;
    int message_data;
};

int main(int argc, char** argv) {
    int message_qid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);

    int pid = fork();

    if (pid < 0) {
        // If the pid < 0, then an error has occured
        fprintf(stderr, "Forking a child process has failed.");
    } else if (pid == 0) {
        // If the pid == 0, then the process we're in is the child
        struct message M2;
        msgrcv(message_qid, &M2, sizeof(int), 42, 0);
        printf("Received the message: %d!\n", M2.message_data);
        msgctl(message_qid, IPC_RMID, NULL);
    } else {
        // If the pid > 0, then the process we're in is the parent
        struct message M1;
        M1.message_type = 42;
        M1.message_data = 252;
        msgsnd(message_qid, &M1, sizeof(int), 0);
    }
    return 0;
}
