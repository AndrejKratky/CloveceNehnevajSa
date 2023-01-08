#include "server.h"

void cakajNaHraca(char* buffer, int newsockfd) {
    citajOdHraca(buffer, newsockfd);
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
    srand(time(NULL));

    STRUKTURA *dataServer = (STRUKTURA *) args;

    POLICKO* hraciaPlocha[11];
    inicializaciaHracejPlochy(hraciaPlocha);
    nastavHraciuPlochuPRAZDE(hraciaPlocha);
    nastavHraciuPlochu(hraciaPlocha);

    printf("Cakam kym sa pripoja vsetci hraci...\n");
    pthread_mutex_lock(dataServer->mutex);
    while(dataServer->pocetZapisanychHracov != dataServer->pocetHracov) {
        pthread_cond_wait(dataServer->zaciatokHry, dataServer->mutex);
    }
    pthread_mutex_unlock(dataServer->mutex);

    int nahodnyHracID = dajNahodneCisloVRozsahu(1, dataServer->pocetHracov);
    HRAC* hracNaTahu = dajHraca(dataServer->hraci, dataServer->pocetHracov, nahodnyHracID);
    nastavHraciuPlochuPRAZDE(hraciaPlocha);
    nastavHraciuPlochu(hraciaPlocha);

    int koniecHry = 0;
    int naTahuID = nahodnyHracID;
    int hodKockou;
    int inputHryCislo;
    int poziciaRiadokHraciaPlocha;
    int poziciaStlpecHraciaPlocha;
    int celkovySucetPriHode6;
    int pocetFigurokNaHracejPloche;
    int hracHodil6;
    int posun;


    while (koniecHry == 0) {
        vycistiKonzolu();
        vykresliHraciuPlochu(hraciaPlocha);
        hracNaTahu = dajHraca(dataServer->hraci, dataServer->pocetHracov, naTahuID);

        switch (skontrolujStavHraca(hracNaTahu)) {
            case 1:
                koniecHry = 1;
                pthread_mutex_lock(dataServer->mutex);
                dataServer->koniecHry = 1;
                pthread_cond_signal(dataServer->condKoniecHry);
                pthread_mutex_unlock(dataServer->mutex);
                break;

            case 2:
                printf("Na tahu: %s\n", hracNaTahu->meno);
                printf("Farba hraca: %s\n", dajFarbuHraca(hracNaTahu));
                printf("Hrac %s nema na hracej ploche ziadnu figurku!\n", hracNaTahu->meno);
                vypisHracovi("Si na tahu.\nNemas na hracej ploche ziadnu figurku.", hracNaTahu->newsockfd, "w");
                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                vypisHracovi("Hadzes kockou...", hracNaTahu->newsockfd, "w");
                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);

                for (int i = 0; i < 3; i++) {
                    vypisHracovi("Pre hod stlac ENTER...", hracNaTahu->newsockfd, "r");
                    citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);
                    hodKockou = dajNahodneCisloVRozsahu(1,6);
                    printf("Hrac %s hadze kockou: %d\n", hracNaTahu->meno, hodKockou);
                    bzero(dataServer->buffer, 256);
                    sprintf(dataServer->buffer, "Hodil si %d", hodKockou);
                    vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                    cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);

                    if (hodKockou == 6) {
                        printf("Hrac %s hodil 6! Presuva figurku na startovacie policko!\n", hracNaTahu->meno);
                        vypisHracovi("Presuvas figurku na startovacie policko.", hracNaTahu->newsockfd, "w");
                        cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
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
                vypisHracovi("Koniec tahu.", hracNaTahu->newsockfd, "w");
                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                naTahuID++;
                if (naTahuID > dataServer->pocetHracov) {
                    naTahuID = 1;
                }
                break;

            case 3:
                printf("Na tahu: %s\n", hracNaTahu->meno);
                printf("Farba hraca: %s\n", dajFarbuHraca(hracNaTahu));
                printf("Hrac vykonava tah...\n");
                vypisHracovi("Si na tahu.\nPre hod stlac ENTER...", hracNaTahu->newsockfd, "r");
                citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);
                hodKockou = dajNahodneCisloVRozsahu(1,6);
                printf("Hrac %s hadze kockou: %d\n", hracNaTahu->meno, hodKockou);
                bzero(dataServer->buffer, 256);
                sprintf(dataServer->buffer, "Hodil si %d", hodKockou);
                vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);

                hracHodil6 = 0;
                celkovySucetPriHode6 = 0;
                if (hodKockou == 6) {
                    hracHodil6 = 1;
                }

                pocetFigurokNaHracejPloche = dajPocetFigurokNaHracejPloche(hracNaTahu);
                if (pocetFigurokNaHracejPloche > 1) {
                    if (hracHodil6 == 1) {
                        vypisHracovi("Mozes si vybrat medzi posunutim a pridanim dalsej figurky na hraciu plochu.\n1. posunutie figurky\n2. pridanie figurky\nZADAJ VOLBU:", hracNaTahu->newsockfd, "r");
                        citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);
                        inputHryCislo = atoi(&dataServer->buffer[0]);
                        printf("input od hraca: %d", inputHryCislo);
                        while (inputHryCislo <= 0 || inputHryCislo >= 3) {
                            vypisHracovi("ZADAJTE CISLO OD 1 DO 2.\nOpatovny pokus o zadanie:", hracNaTahu->newsockfd, "r");
                            citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            inputHryCislo = atoi(&dataServer->buffer[0]);
                        }
                        if (inputHryCislo == 1) {
                            celkovySucetPriHode6 = hodKockou;
                            vypisHracovi("Hodil si 6! Hadzes este raz!\nPre hod stlac ENTER...", hracNaTahu->newsockfd, "r");
                            citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            hodKockou = dajNahodneCisloVRozsahu(1,6);

                            while (hodKockou == 6) {
                                celkovySucetPriHode6 += hodKockou;
                                vypisHracovi("Hodil si 6! Hadzes este raz!\nPre hod stlac ENTER...", hracNaTahu->newsockfd, "r");
                                citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);
                                hodKockou = dajNahodneCisloVRozsahu(1,6);
                            }

                            bzero(dataServer->buffer, 256);
                            sprintf(dataServer->buffer, "Hodil si %d", hodKockou);
                            vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                            cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            celkovySucetPriHode6 += hodKockou;
                            vypisHracovi("Na hracej ploche mas viacero figurok:", hracNaTahu->newsockfd, "w");
                            cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);

                            for (int i = 0; i < hracNaTahu->pocetFiguriek; ++i) {
                                bzero(dataServer->buffer, 256);
                                if ( hracNaTahu->figurkyHraca[i].pocetPrejdenychPolicok > -1) {
                                    sprintf(dataServer->buffer, "Figurka s ID %d sa nachadza na %d. policku od startu.", hracNaTahu->figurkyHraca[i].figurkaID, hracNaTahu->figurkyHraca[i].pocetPrejdenychPolicok);
                                    vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                                    cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                                }
                            }

                            vypisHracovi("ZADAJTE ID FIGURKY OD 1 DO 4:", hracNaTahu->newsockfd, "r");
                            citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            inputHryCislo = atoi(&dataServer->buffer[0]);

                            while (inputHryCislo <= 0 || inputHryCislo >= 5 || jeNaHracejPloche(hracNaTahu, inputHryCislo) != 1) {
                                vypisHracovi("Opatovny pokus o zadanie: ", hracNaTahu->newsockfd, "r");
                                citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);
                                inputHryCislo = atoi(&dataServer->buffer[0]);
                                if (jeNaHracejPloche(hracNaTahu, inputHryCislo) == 1) {
                                    break;
                                } else {
                                    bzero(dataServer->buffer, 256);
                                    sprintf(dataServer->buffer, "Figurka s ID %d sa nenachadza na hracej ploche!", inputHryCislo);
                                    vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                                    cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                                    inputHryCislo = 0;
                                }
                            }
                            FIGURKA* posuvanaFigurka = NULL;
                            for (int i = 0; i < hracNaTahu->pocetFiguriek; i++) {
                                if (hracNaTahu->figurkyHraca[i].figurkaID == inputHryCislo) {
                                    posuvanaFigurka = &(hracNaTahu->figurkyHraca[i]);
                                    break;
                                }
                            }
                            vyprazdniPolicko(&hraciaPlocha[posuvanaFigurka->poziciaRiadok][posuvanaFigurka->poziciaStlpec]);
                            posun = presunFigurku(posuvanaFigurka, celkovySucetPriHode6, posuvanaFigurka->figurkaID, hraciaPlocha);

                            if (posun == 1) {
                                poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                                poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                                nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                                sprintf(dataServer->buffer, "Posunul si figurku ID%d o %d.", posuvanaFigurka->figurkaID, celkovySucetPriHode6);
                                vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            } else if (posun == 2) {
                                poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                                poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                                nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                                sprintf(dataServer->buffer, "Posunul si figurku ID%d o %d a vyhodil si nepriatelovi figurku.", posuvanaFigurka->figurkaID, celkovySucetPriHode6);
                                vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            } else if (posun == 3) {
                                poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                                poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                                nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                                sprintf(dataServer->buffer, "Posunul si figurku ID%d o %d a vyhodil si vlastnu figurku.", posuvanaFigurka->figurkaID, celkovySucetPriHode6);
                                vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            } else {
                                poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                                poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                                nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                                sprintf(dataServer->buffer, "Nemozes sa pohnut o %d policok.", celkovySucetPriHode6);
                                vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            }

                        } else {
                            printf("Hrac %s hodil 6! Presuva figurku na startovacie policko!\n", hracNaTahu->meno);
                            vypisHracovi("Presuvas figurku na startovacie policko!", hracNaTahu->newsockfd, "w");
                            cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
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
                        for (int i = 0; i < hracNaTahu->pocetFiguriek; ++i) {
                            bzero(dataServer->buffer, 256);
                            if ( hracNaTahu->figurkyHraca[i].pocetPrejdenychPolicok > -1) {
                                sprintf(dataServer->buffer, "Figurka s ID %d sa nachadza na %d. policku od startu.", hracNaTahu->figurkyHraca[i].figurkaID, hracNaTahu->figurkyHraca[i].pocetPrejdenychPolicok);
                                vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            }
                        }
                        vypisHracovi("ZADAJTE ID FIGURKY OD 1 DO 4:", hracNaTahu->newsockfd, "r");
                        citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);
                        inputHryCislo = atoi(&dataServer->buffer[0]);
                        while (inputHryCislo <= 0 || inputHryCislo >= 5 || jeNaHracejPloche(hracNaTahu, inputHryCislo) != 1) {
                            vypisHracovi("Opatovny pokus o zadanie: ", hracNaTahu->newsockfd, "r");
                            citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            inputHryCislo = atoi(&dataServer->buffer[0]);
                            if (jeNaHracejPloche(hracNaTahu, inputHryCislo) == 1) {
                                break;
                            } else {
                                bzero(dataServer->buffer, 256);
                                sprintf(dataServer->buffer, "Figurka s ID %d sa nenachadza na hracej ploche!", inputHryCislo);
                                vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                                inputHryCislo = 0;
                            }
                        }
                        FIGURKA* posuvanaFigurka = NULL;
                        for (int i = 0; i < hracNaTahu->pocetFiguriek; i++) {
                            if (hracNaTahu->figurkyHraca[i].figurkaID == inputHryCislo) {
                                posuvanaFigurka = &(hracNaTahu->figurkyHraca[i]);
                                break;
                            }
                        }

                        vyprazdniPolicko(&hraciaPlocha[posuvanaFigurka->poziciaRiadok][posuvanaFigurka->poziciaStlpec]);
                        posun = presunFigurku(posuvanaFigurka, hodKockou, posuvanaFigurka->figurkaID, hraciaPlocha);

                        if (posun == 1) {
                            poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                            poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                            nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                            sprintf(dataServer->buffer, "Posunul si figurku ID%d o %d.", posuvanaFigurka->figurkaID, hodKockou);
                            vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                            cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                        } else if (posun == 2) {
                            poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                            poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                            nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                            sprintf(dataServer->buffer, "Posunul si figurku ID%d o %d a vyhodil si nepriatelovi figurku.", posuvanaFigurka->figurkaID, hodKockou);
                            vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                            cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                        } else if (posun == 3) {
                            poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                            poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                            nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                            sprintf(dataServer->buffer, "Posunul si figurku ID%d o %d a vyhodil si vlastnu figurku.", posuvanaFigurka->figurkaID, hodKockou);
                            vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                            cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                        } else {
                            poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                            poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                            nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                            sprintf(dataServer->buffer, "Nemozes sa pohnut o %d policok.", hodKockou);
                            vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                            cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                        }
                    }
                } else {
                    if (hracHodil6 == 1) {
                        vypisHracovi("Mozes si vybrat medzi posunutim a pridanim dalsej figurky na hraciu plochu.\n1. posunutie figurky\n2. pridanie figurky\nZADAJ VOLBU:", hracNaTahu->newsockfd, "r");
                        citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);
                        inputHryCislo = atoi(&dataServer->buffer[0]);
                        printf("input od hraca: %d", inputHryCislo);

                        while (inputHryCislo <= 0 || inputHryCislo >= 3) {
                            vypisHracovi("ZADAJTE CISLO OD 1 DO 2.\nOpatovny pokus o zadanie:", hracNaTahu->newsockfd, "r");
                            citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            inputHryCislo = atoi(&dataServer->buffer[0]);
                        }
                        if (inputHryCislo == 1) {
                            celkovySucetPriHode6 = hodKockou;
                            vypisHracovi("Hodil si 6! Hadzes este raz!\nPre hod stlac ENTER...", hracNaTahu->newsockfd, "r");
                            citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            hodKockou = dajNahodneCisloVRozsahu(1,6);

                            while (hodKockou == 6) {
                                celkovySucetPriHode6 += hodKockou;
                                vypisHracovi("Hodil si 6! Hadzes este raz!\nPre hod stlac ENTER...", hracNaTahu->newsockfd, "r");
                                citajOdHraca(dataServer->buffer, hracNaTahu->newsockfd);
                                hodKockou = dajNahodneCisloVRozsahu(1,6);
                            }

                            bzero(dataServer->buffer, 256);
                            sprintf(dataServer->buffer, "Hodil si %d", hodKockou);
                            vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                            cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            celkovySucetPriHode6 += hodKockou;

                            FIGURKA* posuvanaFigurka = NULL;
                            for (int i = 1; i <= hracNaTahu->pocetFiguriek; i++) {
                                if (jeNaHracejPloche(hracNaTahu, i) == 1) {
                                    posuvanaFigurka = &(hracNaTahu->figurkyHraca[i - 1]);
                                    break;
                                }
                            }

                            vyprazdniPolicko(&hraciaPlocha[posuvanaFigurka->poziciaRiadok][posuvanaFigurka->poziciaStlpec]);
                            posun = presunFigurku(posuvanaFigurka, celkovySucetPriHode6, posuvanaFigurka->figurkaID, hraciaPlocha);

                            if (posun == 1) {
                                poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                                poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                                nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                                sprintf(dataServer->buffer, "Posunul si figurku ID%d o %d.", posuvanaFigurka->figurkaID, celkovySucetPriHode6);
                                vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            } else if (posun == 2) {
                                poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                                poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                                nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                                sprintf(dataServer->buffer, "Posunul si figurku ID%d o %d a vyhodil si nepriatelovi figurku.", posuvanaFigurka->figurkaID, celkovySucetPriHode6);
                                vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            } else if (posun == 3) {
                                poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                                poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                                nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                                sprintf(dataServer->buffer, "Posunul si figurku ID%d o %d a vyhodil si vlastnu figurku.", posuvanaFigurka->figurkaID, celkovySucetPriHode6);
                                vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            } else {
                                poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                                poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                                nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                                sprintf(dataServer->buffer, "Nemozes sa pohnut o %d policok.", celkovySucetPriHode6);
                                vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                            }

                        } else {
                            printf("Hrac %s hodil 6! Presuva figurku na startovacie policko!\n", hracNaTahu->meno);
                            vypisHracovi("Presuvas figurku na startovacie policko!", hracNaTahu->newsockfd, "w");
                            cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
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

                        posun = presunFigurku(posuvanaFigurka, hodKockou, posuvanaFigurka->figurkaID, hraciaPlocha);

                        if (posun == 1) {
                            poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                            poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                            nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                            sprintf(dataServer->buffer, "Posunul si figurku ID%d o %d.", posuvanaFigurka->figurkaID, hodKockou);
                            vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                            cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                        } else if (posun == 2) {
                            poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                            poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                            nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                            sprintf(dataServer->buffer, "Posunul si figurku ID%d o %d a vyhodil si nepriatelovi figurku.", posuvanaFigurka->figurkaID, hodKockou);
                            vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                            cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                        } else if (posun == 3) {
                            poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                            poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                            nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                            sprintf(dataServer->buffer, "Posunul si figurku ID%d o %d a vyhodil si vlastnu figurku.", posuvanaFigurka->figurkaID, hodKockou);
                            vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                            cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                        } else {
                            poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                            poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                            nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca, posuvanaFigurka);
                            sprintf(dataServer->buffer, "Nemozes sa pohnut o %d policok.", hodKockou);
                            vypisHracovi(dataServer->buffer, hracNaTahu->newsockfd, "w");
                            cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);
                        }
                    }
                }
                vypisHracovi("Koniec tahu.", hracNaTahu->newsockfd, "w");
                cakajNaHraca(dataServer->buffer, hracNaTahu->newsockfd);

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

    for (int i = 0; i < 11; ++i) {
        free(hraciaPlocha[i]);
    }

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
            cakajNaHraca(buffer, newsockfd);
            vypisHracovi("Zadajte vase meno: ", newsockfd, "r");
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

    vypisHracovi("Meno je OK", newsockfd, "w");
    cakajNaHraca(buffer, newsockfd);

    pthread_mutex_lock(dataClient->mutex);
    dataClient->pocetZapisanychHracov++;
    if (dataClient->pocetHracov == dataClient->pocetZapisanychHracov) {
        pthread_cond_signal(dataClient->zaciatokHry);
    }
    pthread_mutex_unlock(dataClient->mutex);

    pthread_mutex_lock(dataClient->mutex);
    while(dataClient->koniecHry == 0) {
        pthread_cond_wait( dataClient->condKoniecHry, dataClient->mutex);
    }
    pthread_mutex_unlock(dataClient->mutex);

    vypisHracovi("Hra skoncila.", newsockfd, "x");

    close(newsockfd);
    return NULL;
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
    pthread_cond_t condKoniecHry = PTHREAD_COND_INITIALIZER;
    pthread_cond_t zaciatokHry = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_t serverThread;
    pthread_t clientThreads[pocetHracov];

    HRAC* clientsData = (HRAC*)malloc(pocetHracov * sizeof(HRAC));  // NOT FREED

    STRUKTURA data = {buffer, sockfd, cli_len, cli_addr, pocetHracov, 0,clientsData, 0,&mutex, &zaciatokHry, &condKoniecHry};

    switch (pocetHracov) {
        case 2:
            data.hraci[0].farbaHraca = Cervena;
            data.hraci[1].farbaHraca = Modra;
            break;
        case 3:
            data.hraci[0].farbaHraca = Cervena;
            data.hraci[1].farbaHraca = Modra;
            data.hraci[2].farbaHraca = Zelena;
            break;
        case 4:
            data.hraci[0].farbaHraca = Cervena;
            data.hraci[1].farbaHraca = Modra;
            data.hraci[2].farbaHraca = Zelena;
            data.hraci[3].farbaHraca = Zlta;
            break;
        default:
            break;
    }

    pthread_create(&serverThread, NULL, funServer, &data);

    for (int i = 0; i < pocetHracov; i++) {
        clientsData[i].id = i + 1;
        clientsData[i].pocetFiguriek = 4;
        clientsData[i].newsockfd = -1;

        FIGURKA* figurkyHraca = (FIGURKA*)malloc(4 * sizeof(FIGURKA));
        for (int j = 0; j < 4; ++j) {
            figurkyHraca[j].poziciaRiadok = -1;
            figurkyHraca[j].poziciaStlpec = -1;
            priradCestuFigurke(&figurkyHraca[j], clientsData[i].farbaHraca);
            figurkyHraca[j].figurkaID = j + 1;
            figurkyHraca[j].pocetPrejdenychPolicok = -1;
            switch (clientsData[i].farbaHraca) {
                case Cervena:
                    figurkyHraca[j].farbaFigurky = Cervena;
                    break;
                case Modra:
                    figurkyHraca[j].farbaFigurky = Modra;
                    break;
                case Zelena:
                    figurkyHraca[j].farbaFigurky = Zelena;
                    break;
                case Zlta:
                    figurkyHraca[j].farbaFigurky = Zlta;
                    break;
                default:
                    break;
            }
        }
        clientsData[i].figurkyHraca = figurkyHraca;
        pthread_create(&clientThreads[i], NULL, funClient, &data);
    }

    pthread_join(serverThread, NULL);
    for (int i = 0; i < pocetHracov; i++) {
        pthread_join(clientThreads[i], NULL);
    }

    close(sockfd);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condKoniecHry);
    pthread_cond_destroy(&zaciatokHry);

    for (int i = 0; i < pocetHracov; ++i) {
        free(clientsData->figurkyHraca);
    }
    free(clientsData);

    return 0;
}