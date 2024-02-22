#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_IP "192.168.28.125"   // Google Public DNS服务器地址
#define SERVER_PORT 8153         // DNS服务器端口号

#define BUF_SIZE 512           // 缓冲区大小

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    
    // 创建UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    
    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);
    
    // 发送DNS请求
    const char *hostname = "www.google.com";
    int query_len = strlen(hostname) + 2;
    unsigned char query[query_len];
    memset(query, 0, query_len);
    query[0] = query_len - 1;
    strncpy((char *)&query[1], hostname, query_len - 2);
    
    if (sendto(sockfd, query, query_len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error sending DNS query");
        exit(EXIT_FAILURE);
    }
    
    // 接收DNS响应
    socklen_t addr_len = sizeof(server_addr);
    int recv_len = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
    if (recv_len < 0) {
        perror("Error receiving DNS response");
        exit(EXIT_FAILURE);
    }
    
    // 打印DNS响应内容
    printf("DNS Response:\n");
    for (int i = 0; i < recv_len; i++) {
        printf("%02x ", buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
    
    // 关闭socket
    close(sockfd);
    
    return 0;
}

