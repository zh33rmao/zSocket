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
    
    // printf("connect to server success\n");
    int send_bytes = send(socks, filename, strlen(filename),0);
    // write(socks, filename, sizeof(filename));
    // if file larger than 4096; printf the first 4096 packet;
    char server_recv[4096];
    recv(socks, &server_recv,sizeof(server_recv),0);
    end_time = clock();
    double run_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("server responses: \n");
    printf("%s\n",server_recv);
    printf("Elapsed: %f(s)\n",run_time);
    sleep(1);
    close(socks);
    return 0;
}
