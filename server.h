#ifndef CLOVECENEHNEVAJSA_SERVER_H
#define CLOVECENEHNEVAJSA_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "hra.h"
#include "vypisy.h"

typedef struct struktura {
    char* buffer;
    int sockfd;
    socklen_t cli_len;
    struct sockaddr_in cli_addr;
    int pocetHracov;
    int pocetZapisanychHracov;
    struct hrac* hraci;
    pthread_mutex_t* mutex;
} STRUKTURA;

int server(int argc, char *argv[]);
void* action(void* connection);

#endif
