#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>

int order_number = 0;

struct order {
    int id;
    char *pizzas[16];
    char address[1024];
    char hours[1024];
};

struct order orders[1024];

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

int order(char *pizzas[], char address[], char hours[]) {
    struct order current_order;

    printf("Placing your order!\n");

    current_order.id = ++order_number;
    strcpy(current_order.address, address);
    strcpy(current_order.hours, hours);

    for (int i = 0; i < 16; ++i) {
        if (pizzas[i] != NULL) {
            current_order.pizzas[i] = calloc(1, strlen(pizzas[i]) + 1);
            strcpy(current_order.pizzas[i], pizzas[i]);
            printf("You are ordering the following pizzas: %s\n", pizzas[i]);
        }
    }

    orders[order_number] = current_order;
    printf("Your order was placed! Order number: %i, %s, %s \n", order_number, current_order.address,
           current_order.hours);

    return order_number;
}

void cancel_order() {
    printf("Cancel your order");
}

char **parse(char buffer[], int number_of_string, const char * delimiter) {
    int i = 0;
    char *token = strtok(buffer, delimiter);
    char ** arr = calloc(number_of_string, sizeof(char *));

    while (token != NULL) {
        arr[i] = calloc(1, strlen(token) + 1);
        strcpy(arr[i], token);
        token = strtok(NULL, " ");
        i++;
    }

    return arr;
}

void handle_request(int connfd) {
    char buffer[1024];

    if (recv(connfd, buffer, sizeof(buffer), 0) == -1) {
        perror("Cannot receive message");
        exit(1);
    }

    if (strncmp(buffer, "order", strlen("order")) == 0) {
        char ** order_args = parse(buffer, 4, " ");
        char ** pizzas = parse(order_args[1], 16, ",");
        order(pizzas, order_args[2], order_args[3]);
        exit(0);
    }

    if (strncmp(buffer, "cancel", strlen("cancel")) == 0) {
        return cancel_order();
        exit(0);
    }
}

int main() {
    struct sockaddr_in sockaddr, client_addr;
    int sock, connfd;

    sock = make_socket(8080, &sockaddr);

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

    handle_request(connfd);
    close(sock);
    return 0;
}
