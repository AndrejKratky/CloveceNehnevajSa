#ifndef CLOVECENEHNEVAJSA_HRA_H
#define CLOVECENEHNEVAJSA_HRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include "client.h"
#include "server.h"

typedef enum farba {
    Cervena,
    Modra,
    Zelena,
    Zlta
} FARBA;
typedef struct figurka {
    int figurkaID;
    int poziciaRiadok;
    int poziciaStlpec;
    int cestaFigurky[44][2];
    int pocetPrejdenychPolicok;
    FARBA farbaFigurky;
} FIGURKA;
typedef struct hrac {
    int id;
    char meno[256];
    FIGURKA* figurkyHraca;
    int pocetFiguriek;
    FARBA farbaHraca;
    int newsockfd;
} HRAC;
typedef enum typyPolicok {
    Horizontalne,
    Vertikalne,
    Prazdne,
    ZlavaHore,
    ZdolaDoprava,
    ZlavaDole,
    ZhoraDoprava,
    DomovHore,
    DomovVpravo,
    DomovDole,
    DomovVlavo,
    DomovKoniecHore,
    DomovKoniecVpravo,
    DomovKoniecDole,
    DomovKoniecVlavo,
    Neinicializovane
} TYP_POLICKA;
typedef struct policko {
    int jeNaMneHrac;
    char obsahPolicka[3][3];
    FIGURKA* figurkaHraca;
} POLICKO;

void inicializaciaHracejPlochy(POLICKO* hraciaPlocha[11]);

char* dajRiadokPolicka(POLICKO* policko, int riadok);

void nastavObsahPolicka(POLICKO* policko, TYP_POLICKA p_typPolicka);

void nastavObsahPolickaFigurka(POLICKO* policko, FARBA p_farba, FIGURKA* p_presuvanaFigurka);

void vyprazdniPolicko(POLICKO* policko);

void nastavHraciuPlochuPRAZDE(POLICKO* p_hraciaPlocha[11]);

void nastavHraciuPlochu(POLICKO* p_hraciaPlocha[11]);

void vykresliHraciuPlochu(POLICKO* p_hraciaPlocha[11]);

int dajNahodneCisloVRozsahu(int min, int max);

void vycistiKonzolu();

HRAC* dajHraca(HRAC* zoznamHracov, int pocetHracov,int idHraca);

void priradCestuFigurke(FIGURKA* figurka, FARBA farba);

int presunFigurku(FIGURKA* figurka, int oKolko, int figurkaID, POLICKO* hraciaPlocha[11]);

char* dajFarbuHraca(HRAC* hrac);

int jeNaHracejPloche(HRAC* hrac, int idFigurky);

int dajPocetFigurokNaHracejPloche(HRAC* hrac);

int skontrolujStavHraca(HRAC* hrac);

#endif