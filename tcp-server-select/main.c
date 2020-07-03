#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define PORT 8080
#define CONNECTION_QUEUE 512
#define BUFFER_SIZE 1024

int main() {
    fd_set active_fds;
    fd_set read_fds;
    int sockfd;
    int newfd;
    int reuse = 1;
    char buffer[BUFFER_SIZE] = {0};

    struct sockaddr_in sockaddr;
    struct sockaddr client_addr;

    bzero(&sockaddr, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(PORT);
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Cannot create socket");
        exit(1);
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(int));

    if (bind(sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1) {
        perror("Cannot bind address to the socket");
        exit(1);
    }

    if (listen(sockfd, CONNECTION_QUEUE) == -1) {
        perror("Cannot listen on port 8080");
        exit(1);
    }

    FD_ZERO(&active_fds);
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &active_fds);

    while (1) {
        read_fds = active_fds;

        if (select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) == -1) {
            perror("Error during select operation");
            exit(1);
        }

        // Iterate all over the sockets
        for (int i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &read_fds)) {
                // Handle new connection
                if (i == sockfd) {
                    socklen_t client_addr_len = sizeof(client_addr);

                    if ((newfd = accept(sockfd, &client_addr, &client_addr_len)) == -1) {
                        perror("Error while accepting a new connection");
                    }

                    printf("Accepting a new connection %i\n", newfd);
                    FD_SET (newfd, &active_fds);
                } else {
                    int n;
                    // Handle data from a client
                    if ((n = recv(i, buffer, BUFFER_SIZE, 0)) <= 0) {
                        // Got error or connection closed by client
                        if (n == 0) {
                            // Connection closed from the client
                            printf("Server: socket %d hung up\n", i);
                            close(i);
                            FD_CLR (i, &active_fds);
                        } else {
                            perror("Error while receiving data");
                        }
                    }

                    printf("Receiving data from socket %i: %s\n", i, buffer);
                    // close(i);
                    // FD_CLR (i, &active_fds);
                }
            }
        }
    }
}
