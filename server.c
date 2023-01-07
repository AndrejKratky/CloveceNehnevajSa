#include "server.h"

void cakajNaHraca(char* buffer, int newsockfd, char* sprava) {
    citajOdHraca(buffer, newsockfd);
    if ((int)buffer[0] == (int)'a') {
        vypisHracovi(sprava, newsockfd, "r");
    }
}

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

    printf("Cakam kym sa pripoja vsetci hraci...\n");
    while(dataServer->pocetZapisanychHracov != dataServer->pocetHracov) {
    }

    int nahodnyHracID = dajNahodneCisloVRozsahu(1, dataServer->pocetHracov);
    HRAC* hracNaTahu = dajHraca(dataServer->hraci, dataServer->pocetHracov, nahodnyHracID);
    // printf("Zacina hrac: %s\n", hracNaTahu->meno);
    nastavHraciuPlochuPRAZDE(hraciaPlocha);
    nastavHraciuPlochu(hraciaPlocha);

    int koniecHry = 0;
    int naTahuID = nahodnyHracID;
    int hodKockou;
    char inputHry;
    int inputHryCislo;
    int poziciaRiadokHraciaPlocha;
    int poziciaStlpecHraciaPlocha;
    int hadzeEsteRaz = 0;
    int celkovySucetPriHode6 = 0;
    int pocetFigurokNaHracejPloche = 0;
    int hracHodil6;


    while (koniecHry == 0) {
        vycistiKonzolu();
        vykresliHraciuPlochu(hraciaPlocha);
        hracNaTahu = dajHraca(dataServer->hraci, dataServer->pocetHracov, naTahuID);

        switch (skontrolujStavHraca(hracNaTahu)) {
            case 1:
                koniecHry = 1;
                break;
            case 2:
                printf("Na tahu: %s\n", hracNaTahu->meno);
                printf("Farba hraca: %s\n", dajFarbuHraca(hracNaTahu));
                printf("Hrac %s nema na hracej ploche ziadnu figurku!\n", hracNaTahu->meno);
                vypisHracovi("Si na tahu.\nNemas na hracej ploche ziadnu figurku.\n", hracNaTahu->newsockfd, "w");
                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd, "Hadzes kockou...");
                citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);


                for (int i = 0; i < 3; i++) {
                    vypisHracovi("Pre hod stlac ENTER...", hracNaTahu->newsockfd, "w");
                    cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd, "");
                    citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);
                    hodKockou = dajNahodneCisloVRozsahu(1,6);
                    printf("Hrac %s hadze kockou: %d\n", hracNaTahu->meno, hodKockou);
                    /*
                    bzero(dataServer->buffer, 256);
                    char* msg = "Hodil si ";
                    char cislo = hodKockou;
                    strcat(msg, (char*)hodKockou);
                    dataServer->buffer
                     */
                    vypisHracovi("Hodil si", hracNaTahu->newsockfd, "w");
                    cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd, "");
                    citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);

                    if (hodKockou == 6) {
                        printf("Hrac %s hodil 6! Presuva figurku na startovacie policko!\n", hracNaTahu->meno);

                        vypisHracovi("Hodil si 6, presuvas figurku na startovacie policko.", hracNaTahu->newsockfd, "w");
                        cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd, "");
                        citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);

                        FIGURKA* posuvanaFigurka = NULL;
                        for (int j = 1; j <= hracNaTahu->pocetFiguriek; j++) {
                            if (jeNaHracejPloche(hracNaTahu, j) == 0) {
                                posuvanaFigurka = &(hracNaTahu->figurkyHraca[j - 1]);
                                break;
                            }
                        }

                        presunFigurku(posuvanaFigurka, 1, posuvanaFigurka->figurkaID, hraciaPlocha);
                        poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                        poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                        nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                        break;
                    }
                }

                naTahuID++;
                if (naTahuID > dataServer->pocetHracov) {
                    naTahuID = 1;
                }
                break;
            case 3:
                printf("Na tahu: %s\n", hracNaTahu->meno);
                printf("Farba hraca: %s\n", dajFarbuHraca(hracNaTahu));
                printf("Pre hod stlacte ENTER...\n");
                hodKockou = dajNahodneCisloVRozsahu(1,6);
                scanf("%c", &inputHry);
                printf("Hrac %s hadze kockou: %d\n", hracNaTahu->meno, hodKockou);

                hracHodil6 = 0;
                celkovySucetPriHode6 = 0;
                if (hodKockou == 6) {
                    hracHodil6 = 1;
                }

                pocetFigurokNaHracejPloche = dajPocetFigurokNaHracejPloche(hracNaTahu);
                if (pocetFigurokNaHracejPloche > 1) {
                    if (hracHodil6 == 1) {
                        printf("Hrac hodil 6! Moze si vybrat medzi posunutim a pridanim dalsej figurky na hraciu plochu.\n");
                        printf("1. posunutie figurky\n");
                        printf("2. pridanie figurky\n");
                        printf("ZADAJTE VOLBU: \n");
                        scanf("%d", &inputHryCislo);

                        while (inputHryCislo <= 0 || inputHryCislo >= 3) {
                            printf("ZADAJTE CISLO OD 1 DO 2.\n");
                            printf("Opatovny pokus o zadanie: \n");
                            scanf("%d", &inputHryCislo);
                        }

                        if (inputHryCislo == 1) {
                            celkovySucetPriHode6 = hodKockou;
                            printf("Hrac hodil 6! Hadze este raz!\n");
                            printf("Pre hod stlacte ENTER...\n");
                            hodKockou = dajNahodneCisloVRozsahu(1,6);
                            celkovySucetPriHode6 += hodKockou;
                            scanf("%c", &inputHry);
                            printf("Hrac %s hadze kockou: %d\n", hracNaTahu->meno, hodKockou);
                            printf("Hrac ma na hracej ploche viacero figurok,\n");
                            printf("vybera si podla ID figurky.\n");
                            vstupIDFigurka(&inputHryCislo, hracNaTahu);
                            FIGURKA* posuvanaFigurka = NULL;
                            for (int i = 0; i < hracNaTahu->pocetFiguriek; i++) {
                                if (hracNaTahu->figurkyHraca[i].figurkaID == inputHryCislo) {
                                    posuvanaFigurka = &(hracNaTahu->figurkyHraca[i]);
                                    break;
                                }
                            }
                            vyprazdniPolicko(&hraciaPlocha[posuvanaFigurka->poziciaRiadok][posuvanaFigurka->poziciaStlpec]);
                            presunFigurku(posuvanaFigurka, celkovySucetPriHode6, posuvanaFigurka->figurkaID, hraciaPlocha);
                            poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                            poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                            nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                        } else {
                            printf("Hrac %s hodil 6! Presuva figurku na startovacie policko!\n", hracNaTahu->meno);
                            FIGURKA* posuvanaFigurka = NULL;
                            for (int j = 1; j <= hracNaTahu->pocetFiguriek; j++) {
                                if (jeNaHracejPloche(hracNaTahu, j) == 0) {
                                    posuvanaFigurka = &(hracNaTahu->figurkyHraca[j - 1]);
                                    break;
                                }
                            }
                            presunFigurku(posuvanaFigurka, 1, posuvanaFigurka->figurkaID, hraciaPlocha);
                            poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                            poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                            nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                        }
                    } else {
                        printf("Hrac ma na hracej ploche viacero figurok,\n");
                        printf("vybera si podla ID figurky.\n");
                        vstupIDFigurka(&inputHryCislo, hracNaTahu);
                        FIGURKA* posuvanaFigurka = NULL;
                        for (int i = 0; i < hracNaTahu->pocetFiguriek; i++) {
                            if (hracNaTahu->figurkyHraca[i].figurkaID == inputHryCislo) {
                                posuvanaFigurka = &(hracNaTahu->figurkyHraca[i]);
                                break;
                            }
                        }
                        vyprazdniPolicko(&hraciaPlocha[posuvanaFigurka->poziciaRiadok][posuvanaFigurka->poziciaStlpec]);
                        presunFigurku(posuvanaFigurka, hodKockou, posuvanaFigurka->figurkaID, hraciaPlocha);
                        poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                        poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                        nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                    }
                } else {
                    if (hracHodil6 == 1) {
                        printf("Hrac hodil 6! Moze si vybrat medzi posunutim a pridanim dalsej figurky na hraciu plochu.\n");
                        printf("1. posunutie figurky\n");
                        printf("2. pridanie figurky\n");
                        printf("ZADAJTE VOLBU: \n");
                        scanf("%d", &inputHryCislo);
                        while (inputHryCislo <= 0 || inputHryCislo >= 3) {
                            printf("ZADAJTE CISLO OD 1 DO 2.\n");
                            printf("Opatovny pokus o zadanie: \n");
                            scanf("%d", &inputHryCislo);
                        }
                        if (inputHryCislo == 1) {
                            celkovySucetPriHode6 = hodKockou;
                            printf("Hrac hodil 6! Hadze este raz!\n");
                            printf("Pre hod stlacte ENTER...\n");
                            hodKockou = dajNahodneCisloVRozsahu(1,6);
                            celkovySucetPriHode6 += hodKockou;
                            scanf("%c", &inputHry);
                            printf("Hrac %s hadze kockou: %d\n", hracNaTahu->meno, hodKockou);
                            FIGURKA* posuvanaFigurka = NULL;
                            for (int i = 0; i < hracNaTahu->pocetFiguriek; i++) {
                                if (hracNaTahu->figurkyHraca[i].figurkaID == inputHryCislo) {
                                    posuvanaFigurka = &(hracNaTahu->figurkyHraca[i]);
                                    break;
                                }
                            }
                            vyprazdniPolicko(&hraciaPlocha[posuvanaFigurka->poziciaRiadok][posuvanaFigurka->poziciaStlpec]);
                            presunFigurku(posuvanaFigurka, celkovySucetPriHode6, posuvanaFigurka->figurkaID, hraciaPlocha);
                            poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                            poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                            nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                        } else {
                            printf("Hrac %s hodil 6! Presuva figurku na startovacie policko!\n", hracNaTahu->meno);
                            FIGURKA* posuvanaFigurka = NULL;
                            for (int j = 1; j <= hracNaTahu->pocetFiguriek; j++) {
                                if (jeNaHracejPloche(hracNaTahu, j) == 0) {
                                    posuvanaFigurka = &(hracNaTahu->figurkyHraca[j - 1]);
                                    break;
                                }
                            }
                            presunFigurku(posuvanaFigurka, 1, posuvanaFigurka->figurkaID, hraciaPlocha);
                            poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                            poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                            nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                        }
                    } else {
                        printf("Hrac posuva figurku o %d policok!...\n", hodKockou);
                        FIGURKA* posuvanaFigurka = NULL;
                        for (int i = 1; i <= hracNaTahu->pocetFiguriek; ++i) {
                            if(jeNaHracejPloche(hracNaTahu, i) == 1) {
                                posuvanaFigurka = &(hracNaTahu->figurkyHraca[i - 1]);
                                break;
                            }
                        }
                        vyprazdniPolicko(&hraciaPlocha[posuvanaFigurka->poziciaRiadok][posuvanaFigurka->poziciaStlpec]);
                        presunFigurku(posuvanaFigurka, hodKockou, posuvanaFigurka->figurkaID, hraciaPlocha);
                        poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                        poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                        nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                    }
                }

                naTahuID++;
                if (naTahuID > dataServer->pocetHracov) {
                    naTahuID = 1;
                }
                break;

            default:
                break;
        }
    }

    printf("KONIEC HRY!\n");
    printf("Zvitazil hrac: %s\n", hracNaTahu->meno);

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
    for (int j = 0; j < 256; j++) {
        if (buffer[j] == '\n') {
            buffer[j] = 0;
            break;
        }
    }

    for (int i = 0; i < dataClient->pocetHracov; ++i) {
        if (strcmp(buffer, dataClient->hraci[i].meno) == 0) {
            i = 0;
            vypisHracovi("Zadane meno uz existuje...", newsockfd, "w");   // w - cakaj
            cakajNaHraca(buffer, newsockfd, "Zadajte vase meno: ");
            citajOdHraca(buffer, newsockfd);
            for (int j = 0; j < 256; j++) {
                if (buffer[j] == '\n') {
                    buffer[j] = 0;
                    break;
                }
            }
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

    listen(sockfd, 4);
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