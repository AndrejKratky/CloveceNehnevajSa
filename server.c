#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "server.h"

/*void* action(void* connection) {
    while (1) {
        STRUKTCONN *connData = (STRUKTCONN *) connection;
        bzero(connData->buffer, 256);
        int n = read(connData->newsockfd, connData->buffer, 255);
        if (n < 0) {
            perror("Error reading from socket");
            // return 4;
        }
        printf("Here is the message: %s\n", connData->buffer);

        const char *msg = "I got your message";
        n = write(connData->newsockfd, msg, strlen(msg) + 1);
        if (n < 0) {
            perror("Error writing to socket");
            // return 5;
        }
    }
}*/

void* funServer(void* args) {
    STRUKTCONN *connData = (STRUKTCONN *) args;
    int newsockfd = accept(connData->sockfd, (struct sockaddr*)&connData->cli_addr, &connData->cli_len);

    while(1) {
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            // return 3;
        }

        bzero(connData->buffer, 256);
        int n = read(newsockfd, connData->buffer, 255);
        if (n < 0) {
            perror("Error reading from socket");
            // return 4;
        }
        printf("Here is the message: %s\n", connData->buffer);

        const char *msg = "I got your message";
        n = write(newsockfd, msg, strlen(msg) + 1);
        if (n < 0) {
            perror("Error writing to socket");
            // return 5;
        }
    }
    close(newsockfd);
    close(connData->sockfd);
}


int server(int argc, char *argv[])
{
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[256];

    if (argc < 2)
    {
        fprintf(stderr,"usage %s port\n", argv[0]);
        return 1;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket address");
        return 2;
    }

    listen(sockfd, 5);
    cli_len = sizeof(cli_addr);

    pthread_cond_t generuj = PTHREAD_COND_INITIALIZER;
    pthread_cond_t odober = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    STRUKTCONN connection = {buffer, sockfd, cli_len, cli_addr};

    pthread_t th1;
    pthread_t th2;

    pthread_create(&th1, NULL, funServer, &connection);
    pthread_create(&th2, NULL, funServer, &connection);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    /*close(newsockfd);
    close(sockfd);*/

    return 0;
}