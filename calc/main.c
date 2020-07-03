#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define HELLO_MESSAGE "\nHello from calc server. What function would you calculate:\n\n log \n exp \n\n"
#define CALCULATE_MESSAGE "\nWrite the value that you want to calculate: "

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

double calculate(char * expression, int x) {
    double result;

    if (strncmp(expression, "log", strlen("log")) == 0) {
         result = log(x);
    }

    if (strncmp(expression, "exp", strlen("exp")) == 0) {
        result = exp(x);
    }

    return result;
}

int main() {
    int sock;
    int reuse = 1;
    struct sockaddr_in sockaddr, client_addr;
    char buffer[BUFFER_SIZE] = {0};

    bzero(&sockaddr, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(PORT);
    sockaddr.sin_addr.s_addr = htons(INADDR_ANY);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error during socket creation");
        exit(1);
    }

    // Reuse the address;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(int));

    if (bind(sock, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1) {
        perror("Cannot bind socket address");
        exit(1);
    }

    if (listen(sock, 1024) == -1) {
        perror("Server cannot listen to the given socket");
        exit(1);
    }

    while (1) {
        int connfd;
        socklen_t client_addr_len = sizeof(client_addr);

        if ((connfd = accept(sock, (struct sockaddr *) &client_addr, &client_addr_len)) == -1) {
            perror("Error while accepting new connection");
            exit(1);
        }

        pid_t childpid = fork();

        if (childpid == -1) {
            perror("Cannot fork child process");
            close(connfd);
        }

        if (childpid == 0) {
            printf("Server correctly forked %d\n", getpid());

            // Close the initial socket
            close(sock);

            send(connfd, HELLO_MESSAGE, sizeof(HELLO_MESSAGE), 0);
            receive_data(connfd, buffer);

            char *expression = calloc(4, sizeof(char ));
            if (strncmp(buffer, "log", strlen("log")) == 0) {
                strcpy(expression, "log");
            }

            if (strncmp(buffer, "exp", strlen("exp")) == 0) {
                strcpy(expression, "exp");
            }

            if (strncmp(buffer, "quit", strlen("quit")) == 0) {
                printf("Closing the connection");
                close(connfd);
                exit(0);
            }

            send(connfd, CALCULATE_MESSAGE, sizeof(CALCULATE_MESSAGE), 0);
            receive_data(connfd, buffer);

            int x = atoi(buffer);
            double result = calculate(expression, x);

            snprintf(buffer, BUFFER_SIZE, "\nResult of exp(%i) = %lf\n", x, result);
            send(connfd, buffer, BUFFER_SIZE, 0);

            close(connfd);
            exit(0);
        } else {
            close(connfd);
        }
    }
}
