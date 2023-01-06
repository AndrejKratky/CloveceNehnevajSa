#ifndef CLOVECENEHNEVAJSA_CLIENT_H
#define CLOVECENEHNEVAJSA_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int client(int argc, char *argv[]);
int citaj(char* buffer, int sockfd);
int zapis(char* buffer, int sockfd);

#endif
