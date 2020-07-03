#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024
#define INPUT_SIZE 128

void send_command(int sock, char *command, char *args) {
    char buffer[256] = {0};

    snprintf(buffer, sizeof(buffer), "%s %s", command, args);

    if (send(sock, buffer, sizeof(buffer), 0) == -1) {
        perror("Command cannot be sent");
    }
}

int receive_data(int socket_fd, char *buffer) {
    ssize_t data_read,
    total = 0;

    // Wait until all data is received or the last char is a newline
    while ((data_read = recv(socket_fd, buffer, BUFFER_SIZE, 0)) > 0) {
        if (data_read == 0 || buffer[data_read - 1] == '\n') {
            break;
        }

        if (data_read == -1) {
            perror("Error while receiving message");
            break;
        }

        total += data_read;
    }

    // Add the null terminator to the last position
    buffer[data_read] = '\0';

    return data_read;
}

int main(int argc, char *argv[]) {
    char input[INPUT_SIZE] = {0};
    char response[BUFFER_SIZE] = {0};
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

    // Start the communication with the server
    printf("Username:\n");
    fgets(input, sizeof(input), stdin);
    send_command(sock, "USER", input);
    receive_data(sock, response);
    printf("Response is: %s\n", response);

    printf("Password:\n");
    fgets(input, sizeof(input), stdin);
    send_command(sock, "PASS", input);
    receive_data(sock, response);
    printf("Response is: %s\n", response);

    printf("LIST your emails:\n");
    send_command(sock, "LIST", "");
    receive_data(sock, response);
    printf("Response is: %s\n", response);

    printf("Insert a command: RETR or DEL\n");
    fgets(input, sizeof(input), stdin);

    if (strncmp(input, "RETR", strlen("RETR")) == 0) {
        send_command(sock, "RETR", input);
        receive_data(sock, response);
        printf("Response is: %s\n", response);
    }

    if (strncmp(input, "DELE", strlen("DEL")) == 0) {
        send_command(sock, "DELE", input);
        receive_data(sock, response);
        printf("Response is: %s\n", response);
    }

    close(sock);
    return 0;
}
