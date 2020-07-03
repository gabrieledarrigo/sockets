#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 8080
#define CONNECTION_QUEUE 512

int main() {
    fd_set master;
    fd_set read_fds;
    int fdmax;
    int sockfd;
    int reuse = 1;

    struct sockaddr_in sockaddr;
    struct sockaddr client_addr;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

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

    FD_SET(sockfd, &master);
    fdmax = sockfd + 1;

    int newfd;
    while (1) {
        read_fds = master;

        if (select(fdmax, &read_fds, NULL, NULL, NULL) == -1) {
            perror("Error during select operation");
            exit(1);
        }

        if (FD_ISSET(sockfd, &read_fds)) {
            socklen_t client_addr_len = sizeof(client_addr);
            
            if ((newfd = accept(sockfd, &client_addr, &client_addr_len)) == -1) {
                perror("Error while accepting a new connection");
            }
        }
    }


    return 0;
}
