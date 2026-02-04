#include <stdio.h>      // printf, perror
#include <stdlib.h>     // exit, EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>     // memset
#include <unistd.h>     // close

#include <arpa/inet.h>  // htons, htonl, inet_ntop
#include <netinet/in.h> // struct sockaddr_in, INADDR_ANY
#include <sys/socket.h> // socket, bind, listen, accept
#include <sys/types.h>  // basic system data types
