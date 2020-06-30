#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT_NUMBER 8080

int main() {
    int sock;
    struct sockaddr_in sockaddr, client_addr;

    bzero(&sockaddr, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(PORT_NUMBER);
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error during socket creation");
        exit(EXIT_FAILURE);
    }

    if (bind(sock, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1) {
        perror("Server cannot bind to the socker");
        exit(EXIT_FAILURE);
    }

    if (listen(sock, 1024) == -1) {
        perror("Server cannot listen to the given socket");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int connfd;
        socklen_t client_addr_len = sizeof(client_addr);

        if ((connfd = accept(sock, (struct sockaddr *) &client_addr, &client_addr_len)) == -1) {
            perror("Server accept failed");
            exit(EXIT_FAILURE);
        }

        pid_t childpid = fork();

        if (childpid == -1) {
            close(connfd);
            continue;
        }

        if (childpid == 0)  {
            printf("Server correctly forked %d\n", getpid());
            close(sock); // We don't need the first socket anymore

            send(connfd, "Hello from the server", sizeof("Hello from the server"), 0);
            exit(EXIT_SUCCESS);
        } else {
            close(connfd);
        }
    }
}
