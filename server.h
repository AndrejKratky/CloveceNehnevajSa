#ifndef CLOVECENEHNEVAJSA_SERVER_H
#define CLOVECENEHNEVAJSA_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "server.h"
#include <pthread.h>

typedef struct struktura {
    char string[255];
    int cislo;
    pthread_mutex_t* mutex;
    pthread_cond_t* volne;
    pthread_cond_t* plne;
} STRUKTURA;

typedef struct struktConn {
    char* buffer;
    int sockfd;
    socklen_t cli_len;
    struct sockaddr_in cli_addr;
} STRUKTCONN;

int server(int argc, char *argv[]);
void* action(void* connection);

#endif
