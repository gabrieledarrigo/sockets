#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    errno = 0;

    struct sockaddr_in sockaddr;
    struct hostent *host;
    int sock;
    char *server_address = argv[1];
    int port_number;
    char message[BUFFER_SIZE];

    if (argc < 3) {
        printf("Usage: %s server_name port\n", argv[0]);
        exit(1);
    }

    port_number = (int) strtol(argv[2], NULL, 10);

    if (errno != 0) {
        perror("The provided port is not a valid number");
    }

    printf("Connecting to server with name: %s, on port: %i\n", server_address, port_number);
    host = gethostbyname(server_address);

    if (host == NULL) {
        perror("Cannot resolve address for the provided hostname");
        exit(1);
    }

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(41234);
    bcopy(host->h_addr, &sockaddr.sin_addr, host->h_length);

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock == -1) {
        perror("Error during socket creation");
    }

    while (1) {
        printf("Insert a message: ");
        fgets(message, BUFFER_SIZE, stdin);

        int sent = sendto(sock, message, strlen(message) -1, 0, (struct sockaddr *) &sockaddr, sizeof(sockaddr));

        if (sent < 0) {
            perror("Message cannot be sent to the server");
            exit(1);
        }

        memset(message, 0, BUFFER_SIZE);
        int received = recvfrom(sock, message, BUFFER_SIZE, 0, NULL, NULL);

        if (received < 0) {
            perror("Cannote receive any message from the server");
            exit(1);
        }

        printf("Message from server: %s\n", message);
    }
}
