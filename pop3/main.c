#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8080

void send_command(int sock, char *command, char *args, char *response) {
    char buffer[256] = {0};

    snprintf(buffer, sizeof(buffer), "%s %s", command, args);

    if (send(sock, buffer, sizeof(buffer), 0) == -1) {
        perror("Error during send message");
    }

    if (recv(sock, response, sizeof(response), 0) == -1) {
        perror("Error during receive message");
    }
}

int main(int argc, char *argv[]) {
    char input[128] = {0};
    char response[2048] = {0};
    int sock;
    struct sockaddr_in sockaddr;

    // Check for args length
    if (argc < 3) {
        printf("usage: %s [SERVER NAME]\n", argv[0]);
        exit(1);
    }

    // Prepare the address for the socket
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(SERVER_PORT);
    sockaddr.sin_addr.s_addr = inet_addr(argv[1]);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error during socket creation");
        exit(1);
    }

    if (connect(sock, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1) {
        perror("Cannot connect with the server");
        exit(1);
    }

    printf("Username:\n");
    fgets(input, sizeof(input), stdin);
    send_command(sock, "USER", input, response);
    printf("Response is: %s\n", response);

    printf("Password:\n");
    fgets(input, sizeof(input), stdin);
    send_command(sock, "PASS", input, response);
    printf("Response is: %s\n", response);

    send_command(sock, "LIST", "", response);
    printf("Response is: %s\n", response);

    printf("Insert a command: RETR or DEL\n");
    fgets(input, sizeof(input), stdin);

    if (strncmp(input, "RETR", strlen("RETR")) == 0) {
        send_command(sock, "RETR", input, response);
        printf("Response is: %s\n", response);
    }

    if (strncmp(input, "DELE", strlen("DEL")) == 0) {
        send_command(sock, "DELE", input, response);
        printf("Response is: %s\n", response);
    }

    close(sock);
    return 0;
}
