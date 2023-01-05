#ifndef CLOVECENEHNEVAJSA_SERVER_H
#define CLOVECENEHNEVAJSA_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include "client.h"
#include "server.h"

typedef struct figurka FIGURKA;
typedef enum farbaHraca FARBA_HRACA;
typedef struct hrac HRAC;
typedef enum typyPolicok TYP_POLICKA;
typedef struct policko POLICKO;

char* dajRiadokPolicka(POLICKO* policko, int riadok);

void nastavObsahPolicka(POLICKO* policko, TYP_POLICKA p_typPolicka);

void nastavObsahPolickaFigurka(POLICKO* policko, FARBA_HRACA p_farba, FIGURKA* p_presuvanaFigurka);

void vyprazdniPolicko(POLICKO* policko);

void nastavHraciuPlochuPRAZDE(POLICKO** p_hraciaPlocha);

void nastavHraciuPlochu(POLICKO** p_hraciaPlocha);

void vykresliHraciuPlochu(POLICKO** p_hraciaPlocha);

int dajNahodneCisloVRozsahu(int min, int max);

void vycistiKonzolu();

HRAC* dajHraca(HRAC* zoznamHracov, int pocetHracov,int idHraca);

void priradCestuFigurke(FIGURKA* figurka, FARBA_HRACA farba);

void presunFigurku(FIGURKA* figurka, int oKolko, int figurkaID, POLICKO** hraciaPlocha);

char* dajFarbuHraca(HRAC* hrac);

int jeNaHracejPloche(HRAC* hrac, int idFigurky);

void vstupIDFigurka(int* inputCislo, HRAC* hrac);

int dajPocetFigurokNaHracejPloche(HRAC* hrac);

int skontrolujStavHraca(HRAC* hrac);

int hra(int argc, char* argv[]);

#endif //CLOVECENEHNEVAJSA_SERVER_H