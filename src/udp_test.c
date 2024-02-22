#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#define BUFSIZE 1500
// #define TUN_DEV "/dev/net/tun"

int main() {
    int sockfd;
    struct sockaddr_in dest_addr;
    char buf[BUFSIZE] = "hello tun0 hello tun0 hello tun0 hello tun0 hello tun0 ;;;";

    // // 打开 TUN 设备
    // if ((tun_fd = open(TUN_DEV, O_RDWR)) < 0) {
    //     perror("Failed to open TUN device");
    //     exit(EXIT_FAILURE);
    // }

    // // 设置 TUN 设备
    // struct ifreq ifr;
    // memset(&ifr, 0, sizeof(ifr));
    // ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
    // if (ioctl(tun_fd, TUNSETIFF, (void *)&ifr) < 0) {
    //     perror("Failed to set TUN device");
    //     close(tun_fd);
    //     exit(EXIT_FAILURE);
    // }

    // 创建 UDP 套接字
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failed to create socket");
        // close(tun_fd);
        exit(EXIT_FAILURE);
    }

    // 设置目标地址和端口
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(12345);  // 设置目标端口
    inet_pton(AF_INET, "10.0.0.1", &dest_addr.sin_addr);  // 设置目标 IP 地址

    // 发送数据
    while (1) {
        // ssize_t bytes_read = read(tun_fd, buf, BUFSIZE);
        // if (bytes_read < 0) {
        //     perror("Failed to read from TUN device");
        //     break;
        // }
        ssize_t bytes_read = strlen(buf);
        printf("buf is %ld \n", bytes_read);
        ssize_t bytes_sent = sendto(sockfd, buf, bytes_read, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (bytes_sent < 0) {
            perror("Failed to send data");
            break;
        }
    }

    // 关闭文件描述符
    close(sockfd);
    // close(tun_fd);

    return 0;
}
