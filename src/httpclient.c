#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    char* address = argv[1];

    int socks;
    struct sockaddr_in server_addr;
    socks = socket(AF_INET, SOCK_STREAM, 0);
    if (socks == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    inet_aton(address, (struct in_addr*) &server_addr.sin_addr.s_addr);
    
    int connection_status = connect(socks, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (connection_status == -1)
        printf("connect to server fail\n");
    
    char* requeset = "GET / HTTP/1.1\r\n\r\n";
    char server_recv[8192];
    send(socks, requeset, sizeof(requeset),0);
    recv(socks, &server_recv,sizeof(server_recv),0);

    printf("server responses: %s\n",server_recv);
    close(socks);    
    return 0;
}