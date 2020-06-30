#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    struct sockaddr_in sockaddr;
    struct hostent *host;
    int sock;
    int port = 41234;
    char server_addr[] = "127.0.0.1";
    char buffer[200];
    char message[] = "Hello world";

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);

    host = gethostbyname(server_addr);

    if (host == NULL) {
        perror("Error during hostname resolution");
        exit(1);
    }

    bcopy(host->h_addr, &sockaddr.sin_addr, host->h_length);
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock == -1) {
        perror("Error during hostname resolution");
        exit(1);
    }

    sendto(sock, message, sizeof(message), 0, (struct sockaddr *) &sockaddr, sizeof(sockaddr));

    int received = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &sockaddr, (socklen_t *) sizeof(sockaddr));

    if (received == 0) {
        perror("No message was receied from the server");
        exit(1);
    }

    printf("Received from the server: %s", buffer);
    close(sock);
    return 0;
}
