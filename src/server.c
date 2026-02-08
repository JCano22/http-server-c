#include <stdio.h>      // printf, perror
#include <stdlib.h>     // exit, EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>     // memset
#include <unistd.h>     // close

#include <arpa/inet.h>  // htons, htonl, inet_ntop
#include <netinet/in.h> // struct sockaddr_in, INADDR_ANY
#include <sys/socket.h> // socket, bind, listen, accept
#include <sys/types.h>  // basic system data types

int main(void)
{
    /*Minimal program skeleton:
     * - prints a startup message so we can see it ran
     * - returns a controlled exit code
     */

     printf("server: starting(no networking yet) \n");

     /* Create a TCP socket (IPv4)*/
     // AF_INET = IPv4
     //SOCK_STREAM = TCP/reliable byte sttream

     int server_fd = socket(AF_INET, SOCK_STREAM, 0);
     if(server_fd < 0)
     {
        perror("socket");
        return EXIT_FAILURE;
     }

     /*=================================================================================*/

     /* Set SO_REUSEADDR so we can restart the server quickly after it exits. */
     //setsockop = set socket options
     //SOL_SOCKET = specifying socket level
     
     int optval = 1;
     if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) 
     {
        perror("setsockopt(SO_REUSEADDR)");
        close(server_fd);
        return EXIT_FAILURE;
    }

    /*=================================================================================*/

    /* Build the IPv4 address we want to bind to (0.0.0.0:8080). */
    // memset makes sure garbage values aren't there after addr struct is created
    //htonl = host to network long -- converts from host bit order to network bit order
    //htonl = host to network short

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));          // clear the struct so unused bytes are 0

    addr.sin_family = AF_INET;               // IPv4 address family
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0 = all local interfaces (in network byte order)
    addr.sin_port = htons(8080);             // port 8080 (in network byte order)


    /*=================================================================================*/

    /* Bind the socket to that address+port. */
    /* This block asks the OS to associate your socket (server_fd) with the local IP address and port described by addr — and aborts cleanly if that fails.*/

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        return EXIT_FAILURE;
    }

    /*=================================================================================*/
    /* Mark the socket as a listening socket (ready to accept connections). */

    if (listen(server_fd, 16) < 0) {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}