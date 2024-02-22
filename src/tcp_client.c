#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "10.0.0.2"
#define PORT 12345
#define BUFSIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFSIZE];
    ssize_t bytes_received, bytes_sent;

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Send data to server
    const char *message = "Hello from client!";
    if ((bytes_sent = send(client_fd, message, strlen(message), 0)) == -1) {
        perror("send");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Sent message to server\n");

    // Receive response from server
    if ((bytes_received = recv(client_fd, buffer, BUFSIZE, 0)) == -1) {
        perror("recv");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Received response from server: %.*s\n", (int)bytes_received, buffer);

    // Close socket
    close(client_fd);

    return 0;
}
