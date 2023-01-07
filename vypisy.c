#include "vypisy.h"

void vypisHracovi(char* sprava, int newsockfd, char* signal) {
    char odoslanaSprava[256];
    odoslanaSprava[0] = *signal;
    strcat(odoslanaSprava, sprava);
    int n = write(newsockfd, odoslanaSprava, strlen(odoslanaSprava) + 1);
    if (n < 0) {
        perror("Error writing to socket");
    }
    bzero(odoslanaSprava, 256);
}

void citajOdHraca(char* buffer, int newsockfd) {
    bzero(buffer, 256);
    int n = read(newsockfd, buffer, 255);
    if (n < 0) {
        perror("Error reading from socket");
    }
}

