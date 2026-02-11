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
     printf("server: starting(no networking yet) \n");

     /* Create a TCP socket (IPv4)*/
     // AF_INET = IPv4
     //SOCK_STREAM = TCP/reliable byte sttream

     int server_fd = socket(AF_INET, SOCK_STREAM, 0); //creating socket, saying we want address family -> IPv4, and stating we want TCP

     //if socket() fails, it returns -1
     if(server_fd < 0)
     {
        perror("socket");
        return EXIT_FAILURE;
     }

     /*=================================================================================*/

     /* Set SO_REUSEADDR so we can restart the server quickly after it exits. */
     //setsockop = set socket options
     //SOL_SOCKET = specifying socket level
     //SO_REUSEADDR = Allow the socket to reuse the address/port even if it is in TIME_WAIT
     //optval controls whether SO_REUSEADDR is enabled (1 = ON, 0 = OFF)
     
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

    /*=================================================================================*/

    /* Accept a single incoming client connection. */
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(server_fd,
                       (struct sockaddr *)&client_addr,
                       &client_len);

    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return EXIT_FAILURE;
    }

    /* Read the client's HTTP request (we'll just grab the first chunk). */
    char req_buf[4096];
    ssize_t nread = read(client_fd, req_buf, sizeof(req_buf) - 1);
    if (nread < 0) {
        perror("read");
        close(client_fd);
        close(server_fd);
        return EXIT_FAILURE;
    }

    /*=================================================================================*/

    /* Read HTTP request and send minimal HTTP response */

    /* Null-terminate so we can safely treat it like a C string for printing. */
    req_buf[nread] = '\0';

    /* Print what we received (for learning/debugging). */
    printf("----- HTTP request start -----\n");
    printf("%s", req_buf);
    printf("------ HTTP request end ------\n");

    /* Send a minimal valid HTTP/1.1 response with a small body. */
    const char *body = "It works!\n";
    const char *hdr =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain; charset=utf-8\r\n"
        "Content-Length: 10\r\n"
        "Connection: close\r\n"
        "\r\n";

    /* Write headers, then body. */
    if (write(client_fd, hdr, strlen(hdr)) < 0) {
        perror("write(headers)");
    }
    if (write(client_fd, body, strlen(body)) < 0) {
        perror("write(body)");
    }

    /*=================================================================================*/

    return EXIT_SUCCESS;
}