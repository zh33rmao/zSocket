#include "ztun.h"

#include <net/if.h>
#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if_tun.h>
#include <arpa/inet.h>

#define BUFSIZE 2000
#define TUN_MTU 1420

// 创建 TUN 接口
static int __tun_set_mtu(int udp_fd, struct ifreq *ifr, int mtu)
{
    ifr->ifr_mtu = mtu;
    if(ioctl(udp_fd, SIOCSIFMTU, ifr) <0)
        return -1;
    return 0;
}

static int __tun_set_up(int udp_fd, struct ifreq *ifr)
{
    ifr->ifr_flags |= IFF_UP;
    if(ioctl(udp_fd, SIOCSIFFLAGS, ifr) <0)
        return -1;
    return 0;
}

static int __tun_set_ip4(int udp_fd, struct ifreq *ifr, struct in_addr *ip4_addr)
{
    ((struct sockaddr_in *) &ifr->ifr_addr)->sin_family = AF_INET;
    ((struct sockaddr_in *) &ifr->ifr_addr)->sin_addr = *ip4_addr;
    if(ioctl(udp_fd, SIOCSIFADDR, ifr) <0)
        return -1;
    return 0;
}

static int __tun_set_netmask4(int udp_fd, struct ifreq *ifr, struct in_addr *ip4_netmask)
{
    ((struct sockaddr_in *) &ifr->ifr_netmask)->sin_family = AF_INET;
    ((struct sockaddr_in *) &ifr->ifr_netmask)->sin_addr = *ip4_netmask;
    if(ioctl(udp_fd, SIOCSIFNETMASK, ifr) <0)
        return -1;
    return 0;
}

int tun_alloc(struct in_addr *ip4_addr, struct in_addr *ip4_netmask, int mtu, char dev_name[])
{
    struct ifreq ifr;
    int fd;
    char *tun_dev_name = "tun%d";
    memset(&ifr, 0, sizeof(struct ifreq));
    fd = open("/dev/net/tun", O_RDWR);
    if(fd <= 0)
        return -1;
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
    memcpy(ifr.ifr_name, tun_dev_name, strlen(tun_dev_name) + 1);
    memcpy(dev_name, tun_dev_name, strlen(tun_dev_name) + 1);
    if(ioctl(fd, TUNSETIFF, &ifr) < 0)
    {
        close(fd);
        return -1;
    }
    if(ioctl(fd, TUNSETPERSIST, 0) < 0)
    {
        close(fd);
        return -1;
    }
    int udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(udp_fd <= 0)
    {
        close(fd);
        return -1;
    }
    if(__tun_set_mtu(udp_fd, &ifr, mtu) < 0)
    {
        close(udp_fd);
        close(fd);
        return -1;
    }
    if(__tun_set_ip4(udp_fd, &ifr, ip4_addr) < 0)
    {
        close(udp_fd);
        close(fd);
        return -1;
    }
    if(__tun_set_netmask4(udp_fd, &ifr, ip4_netmask) < 0)
    {
        close(udp_fd);
        close(fd);
        return -1;
    }
    if(__tun_set_up(udp_fd, &ifr) < 0)
    {
        close(udp_fd);
        close(fd);
        return -1;
    }
    close(udp_fd);
    return fd;
}

int main(int argc, char *argv[]) {
    char tun_name[30];
    char buffer[BUFSIZE];
    int tun_fd, nread;
    // struct ifreq ifr;
    char address[16] = "10.0.0.1";
    char key[65] = {0};
    char netmask[16] = "255.255.255.0";
    struct in_addr ip4_addr;
    struct in_addr ip4_netmask;

    if(strlen(netmask) == 0)
    {
        memcpy(netmask, "255.255.255.0", 14);
    }
    if(inet_pton(AF_INET, address, &ip4_addr) < 0)
    {
        fprintf(stderr, "illegal IPv4 addresses\n");
        return 1;
    }
    if(inet_pton(AF_INET, netmask, &ip4_netmask) < 0)
    {
        fprintf(stderr, "illegal netmask addresses\n");
        return 1;
    }
    tun_fd = tun_alloc(&ip4_addr, &ip4_netmask, TUN_MTU, &tun_name);
    if(tun_fd < 0)
    {
        printf("create tun device failed, you should probably run it with root\n");
        return 1;
    }
    // 无限循环读取从 TUN 接口收到的数据包并打印到标准输出
    while (1) {
        nread = read(tun_fd, buffer, BUFSIZE);
        if (nread < 0) {
            perror("Reading from interface");
            close(tun_fd);
            exit(1);
        }
        printf("Read %d bytes from %s\n", nread, tun_name);
        for (int i = 0; i < nread; i++) {
            printf("%02X ", buffer[i]);
        }
        printf("\n");
    }

    return 0;
}


