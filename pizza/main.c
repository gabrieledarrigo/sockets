#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <netinet/in.h>

int make_socket(int port) {
    int sock;
    struct sockaddr_in sockaddr;

    bzero(&sockaddr, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (sock = socket(AF_INET, SOCK_STREAM, 0) == -1) {
        perror("Error during socket creation");
        exit(1);

    }
}


int main() {








}
