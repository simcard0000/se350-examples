#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT "3500"

int main( int argc, char** argv ) {

    // initializing client-related information
    struct sockaddr_in client_addr;
    int client_addr_size = sizeof(struct sockaddr_in);
    int newsockfd;
    int socketfd = (AF_INET, SOCK_STREAM, 0);

    // setting up the server
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2520);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // binding, listening, and accepting a connection
    bind(socketfd, (struct sockaddr*) &server_addr, sizeof(server_addr));
    listen(socketfd, 5);
    newsockfd = accept(socketfd, NULL, NULL);

    /* Do something useful */

    close(newsockfd);
    close(socketfd);
    return 0;
}