// This example code is from: https://www.gnu.org/software/libc/manual/html_node/Datagram-Example.html
// Please let me know if this does not constitute fair use.

#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER  "/tmp/serversocket"
#define MAXMSG  512

int make_named_socket (const char *filename) {
    struct sockaddr_un name;
    int sock;
    size_t size;

    /* Create the socket. */
    sock = socket (PF_LOCAL, SOCK_DGRAM, 0);
    if (sock < 0) {
      perror ("socket");
      exit (EXIT_FAILURE);
    }

    /* Bind a name to the socket. */
    name.sun_family = AF_LOCAL;
    strncpy (name.sun_path, filename, sizeof (name.sun_path));

    /* The size of the address is
       the offset of the start of the filename,
       plus its length,
       plus one for the terminating null byte.
       Alternatively you can just do:
       size = SUN_LEN (&name);
    */
    size = (offsetof (struct sockaddr_un, sun_path) + strlen (name.sun_path) + 1);

    if (bind (sock, (struct sockaddr *) &name, size) < 0) {
        perror ("bind");
        exit (EXIT_FAILURE);
    }

    return sock;
}

int main (void) {
    int sock;
    char message[MAXMSG];
    struct sockaddr_un name;
    size_t size;
    int nbytes;

    /* Remove the filename first, it’s ok if the call fails */
    unlink (SERVER);

    /* Make the socket, then loop endlessly. */
    sock = make_named_socket (SERVER);
    while (1){
        /* Wait for a datagram. */
        size = sizeof (name);
        nbytes = recvfrom (sock, message, MAXMSG, 0, (struct sockaddr *) & name, &size);
        if (nbytes < 0) {
            perror ("recfrom (server)");
            exit (EXIT_FAILURE);
        }
        /* Give a diagnostic message. */
        fprintf (stderr, "Server: got message: %s\n", message);
        /* Bounce the message back to the sender. */
        nbytes = sendto (sock, message, nbytes, 0, (struct sockaddr *) & name, size);
        if (nbytes < 0) {
            perror ("sendto (server)");
            exit (EXIT_FAILURE);
        }
    }
}
