#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define CONNECTIONS_QUEUE 1024
#define BUFFER_SIZE 1024

int main(int argc, char * argv[]) {
    struct sockaddr_in sockaddr;
    int sock;
    int reuse = 1;
    char message[BUFFER_SIZE];

    bzero(&sockaddr, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(PORT);
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

    if (sock == -1) {
        perror("Error during socket creation");
        exit(1);
    }

    if (bind(sock, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1) {
        perror("Cannot bind socket");
        exit(1);
    }

    if (listen(sock, CONNECTIONS_QUEUE) == -1) {
        perror("Error during socket listening");
        exit(1);
    }

    while (1) {
        int conn;
        struct sockaddr_in client_addr;

        socklen_t client_addr_len = sizeof(client_addr);
        conn = accept(sock, (struct sockaddr *) &client_addr,  &client_addr_len);

        printf("Connection received, %i\n", conn);

        if (conn <0) {
            perror("Server accept failed");
            exit(1);
        }

        if (recv(conn, message, strlen(message), 0) == -1) {
            perror("Error during message receive");
            exit(1);
        }

        printf("Received message %s\n", message);
        close(conn);
    }
}