#include "zsocks5.h"

int mian()
{
    int opt;
    while ((opt = getopt(argc, argv, "P:csh:p:")) != EOF) {
    switch (opt) {
        case 'P': config.localPort = atoi(optarg); break;
        case 'c': config.client = 1; break;
        case 's': config.server = 1; break;
        case 'h': config.serverHost = optarg; break;
        case 'p': config.serverPort = atoi(optarg); break;
        }
    }

}

int createListeningSocket(int port) {
    // socket()函数用来创建一个socket，参数可以指定请求协议(TCP/UDP/其他)
    int listeningSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listeningSock < 0) {
        return SERVER_SOCKET_CREATE_ERROR;
    }

    // 给socket设置属性，SO_REUSEADDR表示关闭socket后，仍可继续重用该socket
    int optval;
    setsockopt(listeningSock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);   // htons和htonl是用来将主机字节顺序转换成网络字节顺序
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 绑定socket到本地地址
    if (bind(listeningSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        return SERVER_SOCKET_BIND_ERROR;
    }

  	// 开始监听
    if (listen(listeningSock, 30) != 0) {
        return SERVER_SOCKET_LISTEN_ERROR;
    }

    return listeningSock;
}

void serverLoop(struct Config config, int serverSock) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLength = sizeof(clientAddr);

    while (1) {
        int clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientAddrLength);
        if (clientSock == -1) {
            continue;
        }
        if (fork() == 0) {
            close(serverSock);
            handleClientRequest(config, clientSock);
            exit(0);
        }
        close(clientSock);
    }
}

void handleClientRequest(struct Config config, int clientSock) {
    printf("handle client socket.\n");
    if (validateSock5Connection(clientSock) < 0) {
        return;
    }
    int remoteSock = createSock5Connection(config, clientSock);
    if (remoteSock < 0) {
        return;
    }

    if (fork() == 0) {
        forwardData(clientSock, remoteSock, 0);
        exit(0);
    }
    if (fork() == 0) {
        forwardData(remoteSock, clientSock, 1);
        exit(0);
    }
}

// 转发数据
void forwardData(int srcSock, int dstSock, int encryption) {
    char buffer[8192];
    ssize_t n;
    while ((n = recv(srcSock, buffer, 8000, 0)) > 0) {
        if (send(dstSock, buffer, (size_t)n, 0) < 0) {
            break;
        }
    }
    shutdown(srcSock, SHUT_RDWR);
    shutdown(dstSock, SHUT_RDWR);
}