#ifndef CLOVECENEHNEVAJSA_VYPISY_H
#define CLOVECENEHNEVAJSA_VYPISY_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void vypisHracovi(const char* sprava, int newsockfd, char* signal);
void citajOdHraca(char* buffer, int newsockfd);

#endif
