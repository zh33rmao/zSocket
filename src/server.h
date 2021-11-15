#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVERPORT 44455
#define BUFSIZE 4096
#define SOCKETERROR (-1)
#define SERVERBACKLOG 1
#define PATH_MAX 4096


void * open_file(const char * path);
void* handle_connection(void* p_client_socket);
int check (int status, const char * message);