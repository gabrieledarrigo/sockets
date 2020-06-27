#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>

int make_socket(int port, struct sockaddr_in *sockaddr) {
    int sock;

    bzero(sockaddr, sizeof(*sockaddr));
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = htons(port);
    sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error during socket creation");
        exit(1);
    }

    return sock;
}


int main() {
    struct sockaddr_in sockaddr, client_addr;
    int sock, connfd;
    char buffer[1024];

    sock = make_socket(8081, &sockaddr);

    if (bind(sock, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1) {
        perror("Error during bind operation");
        exit(1);
    }

    if (listen(sock, 1024) == -1) {
        perror("Server cannot listen");
        exit(1);
    }

    socklen_t client_addr_len = sizeof(client_addr);
    connfd = accept(sock, (struct sockaddr *) &client_addr, &client_addr_len);

    if (connfd == -1) {
        perror("Server accept failed");
        exit(1);
    }

    if (recv(connfd, buffer, sizeof(buffer), 0) == -1) {
        perror("Cannot receive message");
        exit(1);
    }

    printf("Received first message %s\n", buffer);

    if (recv(connfd, buffer, sizeof(buffer), 0) == -1) {
        perror("Cannot receive message");
        exit(1);
    }

    printf("Received second message %s\n", buffer);

    close(sock);
    return 0;
}
