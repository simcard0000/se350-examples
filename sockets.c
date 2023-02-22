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
#define PLANS_FILE "<put the name of a .dat file here>"

// This code is based off of Jeff Zarnett's "SE 350 Exercise: Rogue One: A Star Wars Story". 
// If re-sharing this code is not allowed per University of Waterloo guidelines, please let me know.

typedef struct {
    char * data;
    int length;
} buffer;

extern int errno;

/*  The load_plans function loads a file. It does not take any arguments,
    but returns a buffer structure with the data. It is the
    responsibility of the caller to deallocate the data element inside 
    that structure.
*/
buffer load_plans( );
char* plansdata;

int sendall(int socket, char *buf, int  *len) {
    int total = 0;
    int bytesleft = *len;
    int n;

    while (total < *len) {
        n = send(socket, buf + total, bytesleft, 0);
        if (n == -1) {
            break;
        }
        total += n;
        bytesleft -= n;
    }
    *len = total;
    return n == -1 ? -1 : 0;
}

int main( int argc, char** argv ) {

    if ( argc != 2 ) {
        printf( "Usage: %s IP-Address\n", argv[0] );
        return -1;
    }
    printf("Planning to connect to %s.\n", argv[1]);

    buffer buf = load_plans();

    struct addrinfo hints;
    struct addrinfo *res;

    int sockfd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo (argv[1], PORT, &hints, &res);
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    int status = connect(sockfd, res->ai_addr, res->ai_addrlen);
    printf("Connection status: %d\n", status);

    int status2 = sendall(sockfd, buf.data, &buf.length);
    printf("Sendall status: %d\n", status2);

    char* response = malloc(50 * sizeof(char));

    int status3 = recv(sockfd, response, 50, 0);
    printf("Receiving status: %d, and the message is: %s", status3, response);

    close(sockfd);
    free(response);
    free(plansdata);
    free(res);

    return 0;
}

buffer load_plans( ) {
    struct stat st;
    stat( PLANS_FILE, &st );
    ssize_t filesize = st.st_size;
    plansdata = malloc( filesize );
    int fd = open( PLANS_FILE, O_RDONLY );
    memset( plansdata, 0, filesize );
    read( fd, plansdata, filesize );
    close( fd );

    buffer buf;
    buf.data = plansdata;
    buf.length = filesize;

    return buf;
}
