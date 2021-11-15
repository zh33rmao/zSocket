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
        // handle_connection(client_socket);
        
        pthread_t t;
        int *pclient  = malloc(sizeof(int));
        *pclient = client_socket;
        pthread_create(&t, NULL, handle_connection, pclient);
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

void* handle_connection(void* p_client_socket)
{
    int client_socket = *(int *)p_client_socket;
    free(p_client_socket);// we dont need this any more
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
        return NULL;
    }

    FILE * fp = fopen(actualpath, "r");
    if (fp == NULL)
    {
        printf("ERROR open file!!!");
        close(client_socket);
        return NULL;
    }

    while ((bytes_read = fread(buffer, 1 ,BUFSIZE, fp)) > 0)
    {
        printf("sending %zu bytes\n", bytes_read);
        write(client_socket, buffer, bytes_read);
    }

    close(client_socket);
    fclose(fp);
    printf("closing connection\n");
    return NULL;
}