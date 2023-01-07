#include "client.h"

int zapis(char* buffer, int sockfd) {
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    int n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        perror("Error writing to socket");
        return 5;
    }
    return 0;
}

int citaj(char* buffer, int sockfd) {
    bzero(buffer, 256);
    int n = read(sockfd, buffer, 255);
    if (n < 0) {
        perror("Error reading from socket");
        return 6;
    }

    char signal = buffer[0];
    char sprava[255];
    for (int i = 0; i < 255; ++i) {
        sprava[i] = buffer[i + 1];
    }
    printf("%s\n", sprava);

    if ((int)signal == (int)'r') { // read
        zapis(buffer, sockfd);
    } else if ((int)signal == (int)'w') {  // wait
        int n = write(sockfd, "a", 2);
        if (n < 0) {
            perror("Error writing to socket");
            return 5;
        }
        return 0;
    } else {
        return 0;
    }
    return 0;
}

int client(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    char buffer[256];

    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        return 1;
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(atoi(argv[2]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 3;
    }

    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error connecting to socket");
        return 4;
    }

    while (1) {
        citaj(buffer, sockfd);
    }

    close(sockfd);

    return 0;
}