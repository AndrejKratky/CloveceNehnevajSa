#include "server.h"

HRAC* getHracSock(int newsockfd, STRUKTURA* dataServer) {
    for (int i = 0; i < dataServer->pocetHracov; ++i) {
        if (dataServer->hraci[i].newsockfd == newsockfd) {
            return &dataServer->hraci[i];
        }
    }
    return NULL;
}

void* funServer(void* args) {
    STRUKTURA *dataServer = (STRUKTURA *) args;

    switch (dataServer->pocetHracov) {
        case 2:
            dataServer->hraci[0].farbaHraca = Cervena;
            dataServer->hraci[1].farbaHraca = Modra;
            break;
        case 3:
            dataServer->hraci[0].farbaHraca = Cervena;
            dataServer->hraci[1].farbaHraca = Modra;
            dataServer->hraci[2].farbaHraca = Zelena;
            break;
        case 4:
            dataServer->hraci[0].farbaHraca = Cervena;
            dataServer->hraci[1].farbaHraca = Modra;
            dataServer->hraci[2].farbaHraca = Zelena;
            dataServer->hraci[3].farbaHraca = Zlta;
            break;
        default:
            break;
    }

    POLICKO* hraciaPlocha[11];
    inicializaciaHracejPlochy(hraciaPlocha);
    nastavHraciuPlochuPRAZDE(hraciaPlocha);
    nastavHraciuPlochu(hraciaPlocha);

    // Caka na zapis mien vsetkych hracov
    pthread_mutex_lock(dataServer->mutex);
    /*while(dataServer->) {

    }*/
    pthread_mutex_unlock(dataServer->mutex);

    int nahodyHracID = dajNahodneCisloVRozsahu(1,dataServer->pocetHracov);
    HRAC* hracNaTahu = dajHraca(dataServer->hraci, dataServer->pocetHracov, nahodyHracID);
    // printf("Zacina hrac: %s\n", hracNaTahu->meno);
    nastavHraciuPlochuPRAZDE(hraciaPlocha);
    nastavHraciuPlochu(hraciaPlocha);

    int koniecHry = 0;
    int naTahuID = nahodyHracID;
    int hodKockou;
    char inputHry;
    int inputHryCislo;
    int poziciaRiadokHraciaPlocha;
    int poziciaStlpecHraciaPlocha;
    int hadzeEsteRaz = 0;
    int celkovySucetPriHode6 = 0;
    int pocetFigurokNaHracejPloche = 0;
    int hracHodil6;

    return NULL;
}

void* funClient(void* args) {
    char buffer[256];
    STRUKTURA *dataClient = (STRUKTURA *) args;
    int newsockfd = accept(dataClient->sockfd, (struct sockaddr*)&dataClient->cli_addr, &dataClient->cli_len);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        return NULL;
    }

    pthread_mutex_lock(dataClient->mutex);
    for (int i = 0; i < dataClient->pocetHracov; ++i) {
        if (dataClient->hraci[i].newsockfd == -1) {
            dataClient->hraci[i].newsockfd = newsockfd;
            break;
        }
    }
    pthread_mutex_unlock(dataClient->mutex);

    HRAC* hrac = getHracSock(newsockfd, dataClient);

    /* INICIALIZACIA PODLA INPUT-OV OD KLIENTOV */
    /*char* zapisaniHraci[dataClient->pocetHracov];
    for(int i = 0; i < dataClient->pocetHracov; i++) {
        zapisaniHraci[i] = (char*)calloc(20, sizeof(char));
    }*/

    vypisHracovi("Zadajte vase meno: ", newsockfd, "r");    // r - hned citaj
    citajOdHraca(buffer, newsockfd);

    for (int i = 0; i < dataClient->pocetHracov; ++i) {
        if (strcmp(buffer, dataClient->hraci[i].meno) == 0) {
            vypisHracovi("Zadane meno uz existuje...\n", newsockfd, "w");   // w - cakaj
            i = 0;
            vypisHracovi("Zadajte vase meno: ", newsockfd, "r");
            citajOdHraca(buffer, newsockfd);
            strcpy(hrac->meno, buffer);
        }
    }
    strcpy(hrac->meno, buffer);
    pthread_mutex_lock(dataClient->mutex);
    dataClient->pocetZapisanychHracov++;
    pthread_mutex_unlock(dataClient->mutex);

    vypisHracovi(hrac->meno, newsockfd, "w");
    while(1) {

    }

    /* HRA */
    while(1) {
        bzero(dataClient->buffer, 256);
        int n = read(newsockfd, dataClient->buffer, 255);
        if (n < 0) {
            perror("Error reading from socket");
            return NULL;
        }
        printf("Here is the message: %s\n", dataClient->buffer);

        const char *msg = "I got your message";
        n = write(newsockfd, msg, strlen(msg) + 1);
        if (n < 0) {
            perror("Error writing to socket");
            return NULL;
        }
    }
    close(newsockfd);
    close(dataClient->sockfd);
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

    int pocetHracov = 0;
    if (argc == 3 && atoi(argv[2]) <= 4 && atoi(argv[2]) >= 2) {
        pocetHracov = atoi(argv[2]);
    } else {
        perror("Nespravne parametre");
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

    /*--------------------------------------------------------------------------------------------------------*/
    //pthread_cond_t generuj = PTHREAD_COND_INITIALIZER;
    //pthread_cond_t odober = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_t serverThread;
    pthread_t clientThreads[pocetHracov];

    HRAC* clientsData = (HRAC*)malloc(pocetHracov * sizeof(HRAC));  // NOT FREED

    STRUKTURA data = {buffer, sockfd, cli_len, cli_addr, pocetHracov, 0,clientsData, &mutex};
    pthread_create(&serverThread, NULL, funServer, &data);
    for (int i = 0; i < pocetHracov; i++) {
        clientsData[i].id = i + 1;
        clientsData[i].pocetFiguriek = 4;
        clientsData[i].pocetFiguriekVCieli = 0;
        clientsData[i].newsockfd = -1;

        FIGURKA* figurkyHraca = (FIGURKA*)malloc(4 * sizeof(FIGURKA));
        for (int j = 0; j < 4; ++j) {
            figurkyHraca[j].poziciaRiadok = -1;
            figurkyHraca[j].poziciaStlpec = -1;
            priradCestuFigurke(&figurkyHraca[j], clientsData[i].farbaHraca);
            figurkyHraca[j].figurkaID = j + 1;
            figurkyHraca[j].pocetPrejdenychPolicok = -1;
        }
        clientsData[i].figurkyHraca = figurkyHraca;
        pthread_create(&clientThreads[i], NULL, funClient, &data);
    }

    pthread_join(serverThread, NULL);
    for (int i = 0; i < pocetHracov; i++) {
        pthread_join(clientThreads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    //pthread_cond_destroy(&odober);
    //pthread_cond_destroy(&generuj);

    /*close(newsockfd);
    close(sockfd);*/

    return 0;
}