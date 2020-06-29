#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>

#define SERVER_PORT 8081
#define BUFFER_SIZE 1024
#define MAX_NUMBER_OF_ORDERS 2048
#define MAX_NUMBER_OF_PIZZA 16

int order_number = 0;

struct order {
    int id;
    char *pizzas[MAX_NUMBER_OF_PIZZA];
    char address[256];
    char hours[5];
};

struct order orders[MAX_NUMBER_OF_ORDERS];

int make_socket(int port, struct sockaddr_in *sockaddr) {
    int sock;

    bzero(sockaddr, sizeof(*sockaddr));
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = htons(port);
    sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error during socket creation");
        exit(EXIT_FAILURE);
    }

    return sock;
}

int order(char *pizzas[], char address[], char hours[]) {
    struct order current_order;

    current_order.id = ++order_number;
    strcpy(current_order.address, address);
    strcpy(current_order.hours, hours);

    for (int i = 0; i < MAX_NUMBER_OF_PIZZA; ++i) {
        if (pizzas[i] != NULL) {
            current_order.pizzas[i] = calloc(1, strlen(pizzas[i]) + 1);
            strcpy(current_order.pizzas[i], pizzas[i]);
        }
    }

    orders[order_number] = current_order;

    return order_number;
}

void cancel_order(int number) {

}

char **parse(char buffer[], int number_of_string, const char * delimiter) {
    int i = 0;
    char *token = strtok(buffer, delimiter);
    char ** arr = calloc(number_of_string, sizeof(char *));

    while (token != NULL) {
        token[strcspn(token, "\n")] = 0; // Remove trailing new lines
        arr[i] = calloc(1, strlen(token) + 1);
        strcpy(arr[i], token);
        token = strtok(NULL, delimiter);
        i++;
    }

    return arr;
}

void handle_order(int connfd, char * request) {
    char buffer[BUFFER_SIZE];
    char ** order_args = parse(request, 4, " ");
    char ** pizzas = parse(order_args[1], 16, ",");

    bzero(buffer, BUFFER_SIZE);
    strcpy(buffer, "\nWe are placing your order...\n");
    send(connfd, buffer, sizeof(buffer), 0);
    int order_n = order(pizzas, order_args[2], order_args[3]);

    struct order current_order = orders[order_n];

    bzero(buffer, BUFFER_SIZE);
    snprintf(buffer, BUFFER_SIZE, "\nYour order was placed!\nOrder number: %i\nAddress: %s\nDelivery hours: %s\nPizzas:\n", current_order.id, current_order.address, current_order.hours);
    send(connfd, buffer, sizeof(buffer), 0);

    for (int i = 0; i < MAX_NUMBER_OF_PIZZA; ++i) {
        if (pizzas[i] != NULL) {
            bzero(buffer, BUFFER_SIZE);
            snprintf(buffer, BUFFER_SIZE, "\t%s\n", current_order.pizzas[i]);
            send(connfd, buffer, sizeof(buffer), 0);
        }
    }
}

void handle_cancel(int connfd, char * request) {
    char buffer[BUFFER_SIZE];
    char * rest;
    int order_n = (int) strtol(request, &rest, 10);

    if (order_n > MAX_NUMBER_OF_ORDERS) {
        perror("Your order number is wrong");
        exit(EXIT_FAILURE);
    }

    cancel_order(order_n);
    printf("Received order cancellation for order with number: %i", order_n);
}

void handle_request(int connfd) {
    char buffer[BUFFER_SIZE];

    if (recv(connfd, buffer, sizeof(buffer), 0) == -1) {
        perror("Cannot receive message");
        exit(1);
    }

    if (strncmp(buffer, "order", strlen("order")) == 0) {
        handle_order(connfd, buffer);
    }

    if (strncmp(buffer, "cancel", strlen("cancel")) == 0) {
        handle_cancel(connfd, buffer);
    }
}

int main() {
    struct sockaddr_in sockaddr, client_addr;
    int sock, connfd;

    sock = make_socket(SERVER_PORT, &sockaddr);

    if (bind(sock, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1) {
        perror("Error during bind operation");
        exit(EXIT_FAILURE);
    }

    if (listen(sock, 1024) == -1) {
        perror("Server cannot listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        socklen_t client_addr_len = sizeof(client_addr);
        connfd = accept(sock, (struct sockaddr *) &client_addr, &client_addr_len);

        if (connfd == -1) {
            perror("Server accept failed");
            exit(EXIT_FAILURE);
        }

        handle_request(connfd);
        close(connfd);
    }
}
