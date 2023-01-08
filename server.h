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
    int koniecHry;
    pthread_mutex_t* mutex;
    pthread_cond_t* zaciatokHry;
    pthread_cond_t* condKoniecHry;
} STRUKTURA;

void cakajNaHraca(char* buffer, int newsockfd);
int server(int argc, char *argv[]);

#endif
