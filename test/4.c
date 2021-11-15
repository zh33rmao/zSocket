#include "server.h"

int main(int argc, char* argv[])
{
    // char server_message[256] = "You Have Reached The Server!!!";

    int server_socket, client_socket, addr_size;
    struct sockaddr_in server_addr, client_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }
    
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
     
    check(bind_status, "Bind fail!!");

    int listen_status = listen(server_socket, 128);
    check(listen_status, "Listen fail !!!");

    // for (;;)
    while(1)
    {
        printf("Waiting for connection from clients...\n");
        printf("server running on port: %d\n", SERVERPORT);
        addr_size = sizeof(struct sockaddr_in);
        client_socket = accept(server_socket, 
            (struct sockaddr *) &client_addr, (socklen_t*)&addr_size);
        check(client_socket,"Accept Fail!!!");
        printf("Connected\n");
        handle_connection(client_socket);

        // send(client_socket, server_message, sizeof(server_message), 0);
    }

    return 0;
}

int check (int status, const char* message)
{
    if (status == -1)
    {
        perror(message);
        exit(1);
    }
    return status;
}

void * open_file(const char * path)
{
    FILE * fp;

    fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        fclose(fp);
        return NULL;
    }

    return NULL;

}

void handle_connection(int client_socket)
{
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actualpath[PATH_MAX+1];

    //read client message 
    while ((bytes_read = read(client_socket, buffer+msgsize, 
                    sizeof(buffer)-msgsize-1))>0)
    {
        msgsize += bytes_read;
        if (msgsize > BUFSIZE-1 || buffer[msgsize-1] == '\n') break;
    }
    check (bytes_read, "read fail");
    buffer[msgsize -1] = 0;

    printf("client REQUEST file : %s\n", buffer);
    fflush(stdout);

    //validity check
    if (realpath(buffer, actualpath) == NULL)
    {
        printf("ERROR (bad path): %s\n", buffer);
        close(client_socket);
        return;
    }

    FILE * fp = fopen(actualpath, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        close(client_socket);
        return;
    }

    while ((bytes_read = fread(buffer, 1 ,BUFSIZE, fp)) > 0)
    {
        printf("sending %zu bytes\n", bytes_read);
        write(client_socket, buffer, bytes_read);
    }

    close(client_socket);
    fclose(fp);
    printf("closing connection\n");
}

#include "client.h"

int main(int argc, char* argv[])
{
    clock_t start_time, end_time;
    start_time = clock();
    int socks;
    // char* filename = "/home/zdx/repo/mts/client.h\n";
    char* filename = argv[1];
    strcat(filename, "\n");

    // printf("%s", filename);
    struct sockaddr_in server_addr;
    socks = socket(AF_INET, SOCK_STREAM, 0);
    if (socks == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }
    
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(44455);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // bind(socks, (struct sockaddr *) &server_addr, sizeof(s))

    int connection_status = connect(socks, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (connection_status == -1)
        printf("connect to server fail\n");
    
    printf("connect to server success\n");
    int send_bytes = send(socks, filename, strlen(filename),0);
    // write(socks, filename, sizeof(filename));
    char server_recv[4096];
    recv(socks, &server_recv,sizeof(server_recv),0);
    end_time = clock();
    double run_time = (double)((end_time - start_time)/CLOCKS_PER_SEC * 1000);
    // printf("server responses: \n");
    printf("Elapsed: %f(ms)\n",run_time);
    close(socks);    
    return 0;
}

#include "client.h"

int main(int argc, char* argv[])
{
    clock_t start_time, end_time;
    start_time = clock();
    int socks;
    // char* filename = "/home/zdx/repo/mts/client.h\n";
    char* filename = argv[1];
    strcat(filename, "\n");

    // printf("%s", filename);
    struct sockaddr_in server_addr;
    socks = socket(AF_INET, SOCK_STREAM, 0);
    if (socks == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(44455);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // bind(socks, (struct sockaddr *) &server_addr, sizeof(s))

    int connection_status = connect(socks, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (connection_status == -1)
        printf("connect to server fail\n");

    printf("connect to server success\n");
    int send_bytes = send(socks, filename, strlen(filename),0);
    // write(socks, filename, sizeof(filename));
    char server_recv[4096];
    recv(socks, &server_recv,sizeof(server_recv),0);
    end_time = clock();
    double run_time = (double)((end_time - start_time)/CLOCKS_PER_SEC * 1000);
    // printf("server responses: \n");
    printf("Elapsed: %f(ms)\n",run_time);
    close(socks);
    return 0;
}

#include "client.h"

int main(int argc, char* argv[])
{
    clock_t start_time, end_time;
    start_time = clock();
    int socks;
    // char* filename = "/home/zdx/repo/mts/client.h\n";
    char* filename = argv[1];
    strcat(filename, "\n");

    // printf("%s", filename);
    struct sockaddr_in server_addr;
    socks = socket(AF_INET, SOCK_STREAM, 0);
    if (socks == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(44455);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // bind(socks, (struct sockaddr *) &server_addr, sizeof(s))

    int connection_status = connect(socks, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (connection_status == -1)
        printf("connect to server fail\n");

    printf("connect to server success\n");
    int send_bytes = send(socks, filename, strlen(filename),0);
    // write(socks, filename, sizeof(filename));
    char server_recv[4096];
    recv(socks, &server_recv,sizeof(server_recv),0);
    end_time = clock();
    double run_time = (double)((end_time - start_time)/CLOCKS_PER_SEC * 1000);
    // printf("server responses: \n");
    printf("Elapsed: %f(ms)\n",run_time);
    close(socks);
    return 0;
}

#include "client.h"

int main(int argc, char* argv[])
{
    clock_t start_time, end_time;
    start_time = clock();
    int socks;
    // char* filename = "/home/zdx/repo/mts/client.h\n";
    char* filename = argv[1];
    strcat(filename, "\n");

    // printf("%s", filename);
    struct sockaddr_in server_addr;
    socks = socket(AF_INET, SOCK_STREAM, 0);
    if (socks == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(44455);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // bind(socks, (struct sockaddr *) &server_addr, sizeof(s))

    int connection_status = connect(socks, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (connection_status == -1)
        printf("connect to server fail\n");

    printf("connect to server success\n");
    int send_bytes = send(socks, filename, strlen(filename),0);
    // write(socks, filename, sizeof(filename));
    char server_recv[4096];
    recv(socks, &server_recv,sizeof(server_recv),0);
    end_time = clock();
    double run_time = (double)((end_time - start_time)/CLOCKS_PER_SEC * 1000);
    // printf("server responses: \n");
    printf("Elapsed: %f(ms)\n",run_time);
    close(socks);
    return 0;
}

#include "client.h"

int main(int argc, char* argv[])
{
    clock_t start_time, end_time;
    start_time = clock();
    int socks;
    // char* filename = "/home/zdx/repo/mts/client.h\n";
    char* filename = argv[1];
    strcat(filename, "\n");

    // printf("%s", filename);
    struct sockaddr_in server_addr;
    socks = socket(AF_INET, SOCK_STREAM, 0);
    if (socks == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(44455);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // bind(socks, (struct sockaddr *) &server_addr, sizeof(s))

    int connection_status = connect(socks, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (connection_status == -1)
        printf("connect to server fail\n");

    printf("connect to server success\n");
    int send_bytes = send(socks, filename, strlen(filename),0);
    // write(socks, filename, sizeof(filename));
    char server_recv[4096];
    recv(socks, &server_recv,sizeof(server_recv),0);
    end_time = clock();
    double run_time = (double)((end_time - start_time)/CLOCKS_PER_SEC * 1000);
    // printf("server responses: \n");
    printf("Elapsed: %f(ms)\n",run_time);
    close(socks);
    return 0;
}

#include "client.h"

int main(int argc, char* argv[])
{
    clock_t start_time, end_time;
    start_time = clock();
    int socks;
    // char* filename = "/home/zdx/repo/mts/client.h\n";
    char* filename = argv[1];
    strcat(filename, "\n");

    // printf("%s", filename);
    struct sockaddr_in server_addr;
    socks = socket(AF_INET, SOCK_STREAM, 0);
    if (socks == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(44455);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // bind(socks, (struct sockaddr *) &server_addr, sizeof(s))

    int connection_status = connect(socks, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (connection_status == -1)
        printf("connect to server fail\n");

    printf("connect to server success\n");
    int send_bytes = send(socks, filename, strlen(filename),0);
    // write(socks, filename, sizeof(filename));
    char server_recv[4096];
    recv(socks, &server_recv,sizeof(server_recv),0);
    end_time = clock();
    double run_time = (double)((end_time - start_time)/CLOCKS_PER_SEC * 1000);
    // printf("server responses: \n");
    printf("Elapsed: %f(ms)\n",run_time);
    close(socks);
    return 0;
}

#include "client.h"

int main(int argc, char* argv[])
{
    clock_t start_time, end_time;
    start_time = clock();
    int socks;
    // char* filename = "/home/zdx/repo/mts/client.h\n";
    char* filename = argv[1];
    strcat(filename, "\n");

    // printf("%s", filename);
    struct sockaddr_in server_addr;
    socks = socket(AF_INET, SOCK_STREAM, 0);
    if (socks == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(44455);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // bind(socks, (struct sockaddr *) &server_addr, sizeof(s))

    int connection_status = connect(socks, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (connection_status == -1)
        printf("connect to server fail\n");

    printf("connect to server success\n");
    int send_bytes = send(socks, filename, strlen(filename),0);
    // write(socks, filename, sizeof(filename));
    char server_recv[4096];
    recv(socks, &server_recv,sizeof(server_recv),0);
    end_time = clock();
    double run_time = (double)((end_time - start_time)/CLOCKS_PER_SEC * 1000);
    // printf("server responses: \n");
    printf("Elapsed: %f(ms)\n",run_time);
    close(socks);
    return 0;
}

#include "server.h"

int main(int argc, char* argv[])
{
    // char server_message[256] = "You Have Reached The Server!!!";

    int server_socket, client_socket, addr_size;
    struct sockaddr_in server_addr, client_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));

    check(bind_status, "Bind fail!!");

    int listen_status = listen(server_socket, 128);
    check(listen_status, "Listen fail !!!");

    // for (;;)
    while(1)
    {
        printf("Waiting for connection from clients...\n");
        printf("server running on port: %d\n", SERVERPORT);
        addr_size = sizeof(struct sockaddr_in);
        client_socket = accept(server_socket,
            (struct sockaddr *) &client_addr, (socklen_t*)&addr_size);
        check(client_socket,"Accept Fail!!!");
        printf("Connected\n");
        handle_connection(client_socket);

        // send(client_socket, server_message, sizeof(server_message), 0);
    }

    return 0;
}

int check (int status, const char* message)
{
    if (status == -1)
    {
        perror(message);
        exit(1);
    }
    return status;
}

void * open_file(const char * path)
{
    FILE * fp;

    fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        fclose(fp);
        return NULL;
    }

    return NULL;

}

void handle_connection(int client_socket)
{
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actualpath[PATH_MAX+1];

    //read client message
    while ((bytes_read = read(client_socket, buffer+msgsize,
                    sizeof(buffer)-msgsize-1))>0)
    {
        msgsize += bytes_read;
        if (msgsize > BUFSIZE-1 || buffer[msgsize-1] == '\n') break;
    }
    check (bytes_read, "read fail");
    buffer[msgsize -1] = 0;

    printf("client REQUEST file : %s\n", buffer);
    fflush(stdout);

    //validity check
    if (realpath(buffer, actualpath) == NULL)
    {
        printf("ERROR (bad path): %s\n", buffer);
        close(client_socket);
        return;
    }

    FILE * fp = fopen(actualpath, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        close(client_socket);
        return;
    }

    while ((bytes_read = fread(buffer, 1 ,BUFSIZE, fp)) > 0)
    {
        printf("sending %zu bytes\n", bytes_read);
        write(client_socket, buffer, bytes_read);
    }

    close(client_socket);
    fclose(fp);
    printf("closing connection\n");
}

#include "server.h"

int main(int argc, char* argv[])
{
    // char server_message[256] = "You Have Reached The Server!!!";

    int server_socket, client_socket, addr_size;
    struct sockaddr_in server_addr, client_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));

    check(bind_status, "Bind fail!!");

    int listen_status = listen(server_socket, 128);
    check(listen_status, "Listen fail !!!");

    // for (;;)
    while(1)
    {
        printf("Waiting for connection from clients...\n");
        printf("server running on port: %d\n", SERVERPORT);
        addr_size = sizeof(struct sockaddr_in);
        client_socket = accept(server_socket,
            (struct sockaddr *) &client_addr, (socklen_t*)&addr_size);
        check(client_socket,"Accept Fail!!!");
        printf("Connected\n");
        handle_connection(client_socket);

        // send(client_socket, server_message, sizeof(server_message), 0);
    }

    return 0;
}

int check (int status, const char* message)
{
    if (status == -1)
    {
        perror(message);
        exit(1);
    }
    return status;
}

void * open_file(const char * path)
{
    FILE * fp;

    fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        fclose(fp);
        return NULL;
    }

    return NULL;

}

void handle_connection(int client_socket)
{
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actualpath[PATH_MAX+1];

    //read client message
    while ((bytes_read = read(client_socket, buffer+msgsize,
                    sizeof(buffer)-msgsize-1))>0)
    {
        msgsize += bytes_read;
        if (msgsize > BUFSIZE-1 || buffer[msgsize-1] == '\n') break;
    }
    check (bytes_read, "read fail");
    buffer[msgsize -1] = 0;

    printf("client REQUEST file : %s\n", buffer);
    fflush(stdout);

    //validity check
    if (realpath(buffer, actualpath) == NULL)
    {
        printf("ERROR (bad path): %s\n", buffer);
        close(client_socket);
        return;
    }

    FILE * fp = fopen(actualpath, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        close(client_socket);
        return;
    }

    while ((bytes_read = fread(buffer, 1 ,BUFSIZE, fp)) > 0)
    {
        printf("sending %zu bytes\n", bytes_read);
        write(client_socket, buffer, bytes_read);
    }

    close(client_socket);
    fclose(fp);
    printf("closing connection\n");
}

#include "server.h"

int main(int argc, char* argv[])
{
    // char server_message[256] = "You Have Reached The Server!!!";

    int server_socket, client_socket, addr_size;
    struct sockaddr_in server_addr, client_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));

    check(bind_status, "Bind fail!!");

    int listen_status = listen(server_socket, 128);
    check(listen_status, "Listen fail !!!");

    // for (;;)
    while(1)
    {
        printf("Waiting for connection from clients...\n");
        printf("server running on port: %d\n", SERVERPORT);
        addr_size = sizeof(struct sockaddr_in);
        client_socket = accept(server_socket,
            (struct sockaddr *) &client_addr, (socklen_t*)&addr_size);
        check(client_socket,"Accept Fail!!!");
        printf("Connected\n");
        handle_connection(client_socket);

        // send(client_socket, server_message, sizeof(server_message), 0);
    }

    return 0;
}

int check (int status, const char* message)
{
    if (status == -1)
    {
        perror(message);
        exit(1);
    }
    return status;
}

void * open_file(const char * path)
{
    FILE * fp;

    fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        fclose(fp);
        return NULL;
    }

    return NULL;

}

void handle_connection(int client_socket)
{
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actualpath[PATH_MAX+1];

    //read client message
    while ((bytes_read = read(client_socket, buffer+msgsize,
                    sizeof(buffer)-msgsize-1))>0)
    {
        msgsize += bytes_read;
        if (msgsize > BUFSIZE-1 || buffer[msgsize-1] == '\n') break;
    }
    check (bytes_read, "read fail");
    buffer[msgsize -1] = 0;

    printf("client REQUEST file : %s\n", buffer);
    fflush(stdout);

    //validity check
    if (realpath(buffer, actualpath) == NULL)
    {
        printf("ERROR (bad path): %s\n", buffer);
        close(client_socket);
        return;
    }

    FILE * fp = fopen(actualpath, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        close(client_socket);
        return;
    }

    while ((bytes_read = fread(buffer, 1 ,BUFSIZE, fp)) > 0)
    {
        printf("sending %zu bytes\n", bytes_read);
        write(client_socket, buffer, bytes_read);
    }

    close(client_socket);
    fclose(fp);
    printf("closing connection\n");
}

#include "server.h"

int main(int argc, char* argv[])
{
    // char server_message[256] = "You Have Reached The Server!!!";

    int server_socket, client_socket, addr_size;
    struct sockaddr_in server_addr, client_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));

    check(bind_status, "Bind fail!!");

    int listen_status = listen(server_socket, 128);
    check(listen_status, "Listen fail !!!");

    // for (;;)
    while(1)
    {
        printf("Waiting for connection from clients...\n");
        printf("server running on port: %d\n", SERVERPORT);
        addr_size = sizeof(struct sockaddr_in);
        client_socket = accept(server_socket,
            (struct sockaddr *) &client_addr, (socklen_t*)&addr_size);
        check(client_socket,"Accept Fail!!!");
        printf("Connected\n");
        handle_connection(client_socket);

        // send(client_socket, server_message, sizeof(server_message), 0);
    }

    return 0;
}

int check (int status, const char* message)
{
    if (status == -1)
    {
        perror(message);
        exit(1);
    }
    return status;
}

void * open_file(const char * path)
{
    FILE * fp;

    fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        fclose(fp);
        return NULL;
    }

    return NULL;

}

void handle_connection(int client_socket)
{
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actualpath[PATH_MAX+1];

    //read client message
    while ((bytes_read = read(client_socket, buffer+msgsize,
                    sizeof(buffer)-msgsize-1))>0)
    {
        msgsize += bytes_read;
        if (msgsize > BUFSIZE-1 || buffer[msgsize-1] == '\n') break;
    }
    check (bytes_read, "read fail");
    buffer[msgsize -1] = 0;

    printf("client REQUEST file : %s\n", buffer);
    fflush(stdout);

    //validity check
    if (realpath(buffer, actualpath) == NULL)
    {
        printf("ERROR (bad path): %s\n", buffer);
        close(client_socket);
        return;
    }

    FILE * fp = fopen(actualpath, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        close(client_socket);
        return;
    }

    while ((bytes_read = fread(buffer, 1 ,BUFSIZE, fp)) > 0)
    {
        printf("sending %zu bytes\n", bytes_read);
        write(client_socket, buffer, bytes_read);
    }

    close(client_socket);
    fclose(fp);
    printf("closing connection\n");
}

#include "server.h"

int main(int argc, char* argv[])
{
    // char server_message[256] = "You Have Reached The Server!!!";

    int server_socket, client_socket, addr_size;
    struct sockaddr_in server_addr, client_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));

    check(bind_status, "Bind fail!!");

    int listen_status = listen(server_socket, 128);
    check(listen_status, "Listen fail !!!");

    // for (;;)
    while(1)
    {
        printf("Waiting for connection from clients...\n");
        printf("server running on port: %d\n", SERVERPORT);
        addr_size = sizeof(struct sockaddr_in);
        client_socket = accept(server_socket,
            (struct sockaddr *) &client_addr, (socklen_t*)&addr_size);
        check(client_socket,"Accept Fail!!!");
        printf("Connected\n");
        handle_connection(client_socket);

        // send(client_socket, server_message, sizeof(server_message), 0);
    }

    return 0;
}

int check (int status, const char* message)
{
    if (status == -1)
    {
        perror(message);
        exit(1);
    }
    return status;
}

void * open_file(const char * path)
{
    FILE * fp;

    fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        fclose(fp);
        return NULL;
    }

    return NULL;

}

void handle_connection(int client_socket)
{
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actualpath[PATH_MAX+1];

    //read client message
    while ((bytes_read = read(client_socket, buffer+msgsize,
                    sizeof(buffer)-msgsize-1))>0)
    {
        msgsize += bytes_read;
        if (msgsize > BUFSIZE-1 || buffer[msgsize-1] == '\n') break;
    }
    check (bytes_read, "read fail");
    buffer[msgsize -1] = 0;

    printf("client REQUEST file : %s\n", buffer);
    fflush(stdout);

    //validity check
    if (realpath(buffer, actualpath) == NULL)
    {
        printf("ERROR (bad path): %s\n", buffer);
        close(client_socket);
        return;
    }

    FILE * fp = fopen(actualpath, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        close(client_socket);
        return;
    }

    while ((bytes_read = fread(buffer, 1 ,BUFSIZE, fp)) > 0)
    {
        printf("sending %zu bytes\n", bytes_read);
        write(client_socket, buffer, bytes_read);
    }

    close(client_socket);
    fclose(fp);
    printf("closing connection\n");
}

#include "server.h"

int main(int argc, char* argv[])
{
    // char server_message[256] = "You Have Reached The Server!!!";

    int server_socket, client_socket, addr_size;
    struct sockaddr_in server_addr, client_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));

    check(bind_status, "Bind fail!!");

    int listen_status = listen(server_socket, 128);
    check(listen_status, "Listen fail !!!");

    // for (;;)
    while(1)
    {
        printf("Waiting for connection from clients...\n");
        printf("server running on port: %d\n", SERVERPORT);
        addr_size = sizeof(struct sockaddr_in);
        client_socket = accept(server_socket,
            (struct sockaddr *) &client_addr, (socklen_t*)&addr_size);
        check(client_socket,"Accept Fail!!!");
        printf("Connected\n");
        handle_connection(client_socket);

        // send(client_socket, server_message, sizeof(server_message), 0);
    }

    return 0;
}

int check (int status, const char* message)
{
    if (status == -1)
    {
        perror(message);
        exit(1);
    }
    return status;
}

void * open_file(const char * path)
{
    FILE * fp;

    fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        fclose(fp);
        return NULL;
    }

    return NULL;

}

void handle_connection(int client_socket)
{
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actualpath[PATH_MAX+1];

    //read client message
    while ((bytes_read = read(client_socket, buffer+msgsize,
                    sizeof(buffer)-msgsize-1))>0)
    {
        msgsize += bytes_read;
        if (msgsize > BUFSIZE-1 || buffer[msgsize-1] == '\n') break;
    }
    check (bytes_read, "read fail");
    buffer[msgsize -1] = 0;

    printf("client REQUEST file : %s\n", buffer);
    fflush(stdout);

    //validity check
    if (realpath(buffer, actualpath) == NULL)
    {
        printf("ERROR (bad path): %s\n", buffer);
        close(client_socket);
        return;
    }

    FILE * fp = fopen(actualpath, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        close(client_socket);
        return;
    }

    while ((bytes_read = fread(buffer, 1 ,BUFSIZE, fp)) > 0)
    {
        printf("sending %zu bytes\n", bytes_read);
        write(client_socket, buffer, bytes_read);
    }

    close(client_socket);
    fclose(fp);
    printf("closing connection\n");
}

#include "server.h"

int main(int argc, char* argv[])
{
    // char server_message[256] = "You Have Reached The Server!!!";

    int server_socket, client_socket, addr_size;
    struct sockaddr_in server_addr, client_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        printf("create socket fail\n");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));

    check(bind_status, "Bind fail!!");

    int listen_status = listen(server_socket, 128);
    check(listen_status, "Listen fail !!!");

    // for (;;)
    while(1)
    {
        printf("Waiting for connection from clients...\n");
        printf("server running on port: %d\n", SERVERPORT);
        addr_size = sizeof(struct sockaddr_in);
        client_socket = accept(server_socket,
            (struct sockaddr *) &client_addr, (socklen_t*)&addr_size);
        check(client_socket,"Accept Fail!!!");
        printf("Connected\n");
        handle_connection(client_socket);

        // send(client_socket, server_message, sizeof(server_message), 0);
    }

    return 0;
}

int check (int status, const char* message)
{
    if (status == -1)
    {
        perror(message);
        exit(1);
    }
    return status;
}

void * open_file(const char * path)
{
    FILE * fp;

    fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        fclose(fp);
        return NULL;
    }

    return NULL;

}

void handle_connection(int client_socket)
{
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actualpath[PATH_MAX+1];

    //read client message
    while ((bytes_read = read(client_socket, buffer+msgsize,
                    sizeof(buffer)-msgsize-1))>0)
    {
        msgsize += bytes_read;
        if (msgsize > BUFSIZE-1 || buffer[msgsize-1] == '\n') break;
    }
    check (bytes_read, "read fail");
    buffer[msgsize -1] = 0;

    printf("client REQUEST file : %s\n", buffer);
    fflush(stdout);

    //validity check
    if (realpath(buffer, actualpath) == NULL)
    {
        printf("ERROR (bad path): %s\n", buffer);
        close(client_socket);
        return;
    }

    FILE * fp = fopen(actualpath, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        close(client_socket);
        return;
    }

    while ((bytes_read = fread(buffer, 1 ,BUFSIZE, fp)) > 0)
    {
        printf("sending %zu bytes\n", bytes_read);
        write(client_socket, buffer, bytes_read);
    }

    close(client_socket);
    fclose(fp);
    printf("closing connection\n");
}
