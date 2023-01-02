#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

/**
 * Jednotlive typy policok su zapisane v Exceli
 * */

/**
    push test
**/

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

/**
 * Jedno policko, reprezentovne 2D maticou 3x3
 * Kazde policko bude:
 *      - vediet, ci sa na nom nachadza hrac
 *      - mat svoj typ (12 typov)
 * Atributy :
 *      int jeNaMneHrac -> 1(ano) alebo 0(nie)
 * */
typedef struct policko {
    int jeNaMneHrac;
    TYP_POLICKA typPolicka;
    char obsahPolicka[3][3];
} POLICKO;

typedef enum farbaHraca {
    Cervena,
    Modra,
    Zelena,
    Zlta
} FARBA_HRACA;

/**
 * Kazda figurka si bude pamatat svoju cestu do ciela (v ramci hracej plochy)
 * Napr. pre cervenu figurku:
 *      riadok  stlpec
 *      4       0
 *      4       1
 *      4       2
 *      4       3
 *      atd.
 * */
typedef struct figurka {
    int figurkaID;
    int poziciaRiadok;
    int poziciaStlpec;
    int cestaFigurky[44][2];
    int pocetPrejdenychPolicok;
} FIGURKA;

typedef struct hrac {
    int id;
    char meno[20];
    FIGURKA* figurkyHraca;
    int pocetFiguriek;
    int pocetFiguriekVCieli;
    FARBA_HRACA farbaHraca;
} HRAC;

/**
 * Pri vykreslovani na terminal sa hracia plocha generuje po riadkoch, nie po celych polickach.
 * Preto potrebujeme kazde policko rozdelit trikrat.
 * Priklad:
 *      char* riadok = dajRiadokPolicka(&testPolicko, 1);
 *      printf("%s", riadok);
 *      free(riadok);
 *
 * @param riadok - ocakava vstup > 0
 * */
char* dajRiadokPolicka(POLICKO* policko, int riadok) {
    char* riadokPolicka = (char*)calloc(3, sizeof(char));
    for (int i = 0; i < 3; i++) {
        riadokPolicka[i] = policko->obsahPolicka[riadok - 1][i];
    }
    return riadokPolicka;
}

void nastavObsahPolicka(POLICKO* policko, TYP_POLICKA p_typPolicka) {
    if (p_typPolicka == Horizontalne) {
        for (int i = 0; i < 3; ++i) {
            policko->obsahPolicka[0][i] = '-';
            policko->obsahPolicka[2][i] = '-';
        }
        for (int i = 0; i < 3; ++i) {
            policko->obsahPolicka[1][i] = ' ';
        }
        policko->typPolicka = Horizontalne;

    } else if (p_typPolicka == Vertikalne) {
        for (int i = 0; i < 3; ++i) {
            policko->obsahPolicka[i][0] = '|';
            policko->obsahPolicka[i][2] = '|';
        }
        for (int i = 0; i < 3; ++i) {
            policko->obsahPolicka[i][1] = ' ';
        }
        policko->typPolicka = Vertikalne;

    } else if (p_typPolicka == Prazdne) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                policko->obsahPolicka[i][j] = 'X';
            }
        }
        policko->typPolicka = Prazdne;

    } else if (p_typPolicka == ZlavaHore) {
        policko->obsahPolicka[0][0] = '/';
        policko->obsahPolicka[0][1] = ' ';
        policko->obsahPolicka[0][2] = '|';

        policko->obsahPolicka[1][0] = ' ';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = '|';

        policko->obsahPolicka[2][0] = '-';
        policko->obsahPolicka[2][1] = '-';
        policko->obsahPolicka[2][2] = '/';
        policko->typPolicka = Prazdne;

    } else if (p_typPolicka == ZdolaDoprava) {
        policko->obsahPolicka[0][0] = '/';
        policko->obsahPolicka[0][1] = '-';
        policko->obsahPolicka[0][2] = '-';

        policko->obsahPolicka[1][0] = '|';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = ' ';

        policko->obsahPolicka[2][0] = '|';
        policko->obsahPolicka[2][1] = ' ';
        policko->obsahPolicka[2][2] = '/';
        policko->typPolicka = ZdolaDoprava;

    } else if (p_typPolicka == ZlavaDole) {
        policko->obsahPolicka[0][0] = '-';
        policko->obsahPolicka[0][1] = '-';
        policko->obsahPolicka[0][2] = '\\';

        policko->obsahPolicka[1][0] = ' ';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = '|';

        policko->obsahPolicka[2][0] = '\\';
        policko->obsahPolicka[2][1] = ' ';
        policko->obsahPolicka[2][2] = '|';
        policko->typPolicka = ZlavaDole;

    } else if (p_typPolicka == ZhoraDoprava) {
        policko->obsahPolicka[0][0] = '|';
        policko->obsahPolicka[0][1] = ' ';
        policko->obsahPolicka[0][2] = '\\';

        policko->obsahPolicka[1][0] = '|';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = ' ';

        policko->obsahPolicka[2][0] = '\\';
        policko->obsahPolicka[2][1] = '-';
        policko->obsahPolicka[2][2] = '-';
        policko->typPolicka = ZhoraDoprava;

    } else if (p_typPolicka == DomovHore) {
        policko->obsahPolicka[0][0] = '/';
        policko->obsahPolicka[0][1] = '^';
        policko->obsahPolicka[0][2] = '\\';

        policko->obsahPolicka[1][0] = ' ';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = ' ';

        policko->obsahPolicka[2][0] = '-';
        policko->obsahPolicka[2][1] = '-';
        policko->obsahPolicka[2][2] = '-';
        policko->typPolicka = DomovHore;

    } else if (p_typPolicka == DomovVpravo) {
        policko->obsahPolicka[0][0] = '|';
        policko->obsahPolicka[0][1] = ' ';
        policko->obsahPolicka[0][2] = '\\';

        policko->obsahPolicka[1][0] = '|';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = '>';

        policko->obsahPolicka[2][0] = '|';
        policko->obsahPolicka[2][1] = ' ';
        policko->obsahPolicka[2][2] = '/';
        policko->typPolicka = DomovVpravo;

    } else if (p_typPolicka == DomovDole) {
        policko->obsahPolicka[0][0] = '-';
        policko->obsahPolicka[0][1] = '-';
        policko->obsahPolicka[0][2] = '-';

        policko->obsahPolicka[1][0] = ' ';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = ' ';

        policko->obsahPolicka[2][0] = '\\';
        policko->obsahPolicka[2][1] = 'v';
        policko->obsahPolicka[2][2] = '/';
        policko->typPolicka = DomovDole;

    } else if (p_typPolicka == DomovVlavo) {
        policko->obsahPolicka[0][0] = '/';
        policko->obsahPolicka[0][1] = ' ';
        policko->obsahPolicka[0][2] = '|';

        policko->obsahPolicka[1][0] = '<';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = '|';

        policko->obsahPolicka[2][0] = '\\';
        policko->obsahPolicka[2][1] = ' ';
        policko->obsahPolicka[2][2] = '|';
        policko->typPolicka = DomovVlavo;

    } else if (p_typPolicka == DomovKoniecHore) {
        policko->obsahPolicka[0][0] = '|';
        policko->obsahPolicka[0][1] = '-';
        policko->obsahPolicka[0][2] = '|';

        policko->obsahPolicka[1][0] = '|';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = '|';

        policko->obsahPolicka[2][0] = '|';
        policko->obsahPolicka[2][1] = ' ';
        policko->obsahPolicka[2][2] = '|';
        policko->typPolicka = DomovKoniecHore;

    } else if (p_typPolicka == DomovKoniecVpravo) {
        policko->obsahPolicka[0][0] = '-';
        policko->obsahPolicka[0][1] = '-';
        policko->obsahPolicka[0][2] = '-';

        policko->obsahPolicka[1][0] = ' ';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = '|';

        policko->obsahPolicka[2][0] = '-';
        policko->obsahPolicka[2][1] = '-';
        policko->obsahPolicka[2][2] = '-';
        policko->typPolicka = DomovKoniecVpravo;

    } else if (p_typPolicka == DomovKoniecDole) {
        policko->obsahPolicka[0][0] = '|';
        policko->obsahPolicka[0][1] = ' ';
        policko->obsahPolicka[0][2] = '|';

        policko->obsahPolicka[1][0] = '|';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = '|';

        policko->obsahPolicka[2][0] = '|';
        policko->obsahPolicka[2][1] = '-';
        policko->obsahPolicka[2][2] = '|';
        policko->typPolicka = DomovKoniecDole;

    } else if (p_typPolicka == DomovKoniecVlavo) {
        policko->obsahPolicka[0][0] = '-';
        policko->obsahPolicka[0][1] = '-';
        policko->obsahPolicka[0][2] = '-';

        policko->obsahPolicka[1][0] = '|';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = ' ';

        policko->obsahPolicka[2][0] = '-';
        policko->obsahPolicka[2][1] = '-';
        policko->obsahPolicka[2][2] = '-';
        policko->typPolicka = DomovKoniecVlavo;

    } else {
        return;
    }
}

void nastavObsahPolickaFigurka(POLICKO* policko, FARBA_HRACA p_farba) {
    switch (p_farba) {
        case Cervena:
            policko->obsahPolicka[1][1] = 'R';
            break;
        case Modra:
            policko->obsahPolicka[1][1] = 'B';
            break;
        case Zelena:
            policko->obsahPolicka[1][1] = 'G';
            break;
        case Zlta:
            policko->obsahPolicka[1][1] = 'Y';
            break;
        default:
            break;
    }
}

void vyprazdniPolicko(POLICKO* policko) {
    policko->obsahPolicka[1][1] = ' ';
}

/**
 * Stav prazdnych policok sa nemusi aktualizovat, kedze na ne figurka nemoze skocit.
 * Tato funkcia sa zavola len raz v celom programe.
 * */
void nastavHraciuPlochuPRAZDE(POLICKO p_hraciaPlocha[11][11]) {
    /* PRAZDNE POLICKA */
    for (int i = 0; i <= 3; ++i) {
        for (int j = 0; j <= 3; ++j) {
            nastavObsahPolicka(&p_hraciaPlocha[i][j], Prazdne);
        }
    }
    for (int i = 0; i <= 3; ++i) {
        for (int j = 7; j <= 10; ++j) {
            nastavObsahPolicka(&p_hraciaPlocha[i][j], Prazdne);
        }
    }
    for (int i = 7; i <= 10; ++i) {
        for (int j = 0; j <= 3; ++j) {
            nastavObsahPolicka(&p_hraciaPlocha[i][j], Prazdne);
        }
    }
    for (int i = 7; i <= 10; ++i) {
        for (int j = 7; j <= 10; ++j) {
            nastavObsahPolicka(&p_hraciaPlocha[i][j], Prazdne);
        }
    }
    nastavObsahPolicka(&p_hraciaPlocha[5][5], Prazdne);
}

/**
 * Vykona sa vzdy ked:
 *      1. Bola spustena nova hra.
 *      2. Hrac hodil kockou (resp. chce sa posunut na dalsie policko)
 * */
void nastavHraciuPlochu(POLICKO p_hraciaPlocha[11][11]) {
    /* VERTIKALNE */
    for (int i = 1; i <= 3; ++i) {
        for (int j = 4; j <= 6; ++j) {
            nastavObsahPolicka(&p_hraciaPlocha[i][j], Vertikalne);
        }
    }
    for (int i = 7; i <= 9; ++i) {
        for (int j = 4; j <= 6; ++j) {
            nastavObsahPolicka(&p_hraciaPlocha[i][j], Vertikalne);
        }
    }

    /* HORIZONTALNE */
    for (int i = 4; i <= 6; ++i) {
        for (int j = 1; j <= 3; ++j) {
            nastavObsahPolicka(&p_hraciaPlocha[i][j], Horizontalne);
        }
    }
    for (int i = 4; i <= 6; ++i) {
        for (int j = 7; j <= 9; ++j) {
            nastavObsahPolicka(&p_hraciaPlocha[i][j], Horizontalne);
        }
    }

    /* CAST - CERVENY */
    nastavObsahPolicka(&p_hraciaPlocha[4][0], ZdolaDoprava);
    nastavObsahPolicka(&p_hraciaPlocha[5][0], DomovVpravo);
    nastavObsahPolicka(&p_hraciaPlocha[6][0], ZhoraDoprava);

    /* CAST - MODRY */
    nastavObsahPolicka(&p_hraciaPlocha[0][4], ZdolaDoprava);
    nastavObsahPolicka(&p_hraciaPlocha[0][5], DomovDole);
    nastavObsahPolicka(&p_hraciaPlocha[0][6], ZlavaDole);

    /* CAST - ZELENY */
    nastavObsahPolicka(&p_hraciaPlocha[4][10], ZlavaDole);
    nastavObsahPolicka(&p_hraciaPlocha[5][10], DomovVlavo);
    nastavObsahPolicka(&p_hraciaPlocha[6][10], ZlavaHore);

    /* CAST - ZLTY */
    nastavObsahPolicka(&p_hraciaPlocha[10][4], ZhoraDoprava);
    nastavObsahPolicka(&p_hraciaPlocha[10][5], DomovHore);
    nastavObsahPolicka(&p_hraciaPlocha[10][6], ZlavaHore);

    /* STREDNA CAST */
    nastavObsahPolicka(&p_hraciaPlocha[4][4], ZlavaHore);
    nastavObsahPolicka(&p_hraciaPlocha[6][4], ZlavaDole);
    nastavObsahPolicka(&p_hraciaPlocha[4][6], ZhoraDoprava);
    nastavObsahPolicka(&p_hraciaPlocha[6][6], ZdolaDoprava);

    /* KONCE */
    nastavObsahPolicka(&p_hraciaPlocha[4][5], DomovKoniecDole);
    nastavObsahPolicka(&p_hraciaPlocha[5][4], DomovKoniecVpravo);
    nastavObsahPolicka(&p_hraciaPlocha[5][6], DomovKoniecVlavo);
    nastavObsahPolicka(&p_hraciaPlocha[6][5], DomovKoniecHore);
}

void vykresliHraciuPlochu(POLICKO p_hraciaPlocha[11][11]) {
    char* vypis = NULL;

    for (int riadok = 0; riadok < 11; ++riadok) {
        for (int riadokPolicko = 1; riadokPolicko <= 3; ++riadokPolicko) {
            for (int stlpec = 0; stlpec < 11; ++stlpec) {
                vypis = dajRiadokPolicka(&p_hraciaPlocha[riadok][stlpec], riadokPolicko);
                printf("%s", vypis);
                free(vypis);
            }
            printf("\n");
        }
    }
}

int dajNahodneCisloVRozsahu(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

void vycistiKonzolu() {
    for (int i = 0; i < 100; i++) {
        printf("\n");
    }
}

HRAC* dajHraca(HRAC* zoznamHracov, int pocetHracov,int idHraca) {
    for (int i = 0; i < pocetHracov; ++i) {
        if(zoznamHracov[i].id == idHraca) {
            return &zoznamHracov[i];
        }
    }
    return NULL;
}

int skontrolujStavHraca(HRAC* hrac) {
    if(hrac->pocetFiguriekVCieli == 4) {
        return 1;   // hrac vyhral
    }

    int pocetFiguriekMimoHracejPlochy = 0;
    for (int i = 0; i < hrac->pocetFiguriek; i++) {
        if(hrac->figurkyHraca[i].poziciaRiadok == -1 && hrac->figurkyHraca[i].poziciaStlpec == -1) {
            pocetFiguriekMimoHracejPlochy++;
        }
    }
    if (pocetFiguriekMimoHracejPlochy == 4) {
        return 2;   // hrac nema ziadnu figurku na hracej ploche
    }

    return 3;   // hrac ma aspon jednu figurku na hracej ploche
}

void priradCestuFigurke(FIGURKA* figurka, FARBA_HRACA farba) {
    switch (farba) {
        case Cervena:
            figurka->cestaFigurky[0][0] = 4;
            figurka->cestaFigurky[0][1] = 0;

            figurka->cestaFigurky[1][0] = 4;
            figurka->cestaFigurky[1][1] = 1;

            figurka->cestaFigurky[2][0] = 4;
            figurka->cestaFigurky[2][1] = 2;

            figurka->cestaFigurky[3][0] = 4;
            figurka->cestaFigurky[3][1] = 3;

            figurka->cestaFigurky[4][0] = 4;
            figurka->cestaFigurky[4][1] = 4;

            figurka->cestaFigurky[5][0] = 3;
            figurka->cestaFigurky[5][1] = 4;

            figurka->cestaFigurky[6][0] = 2;
            figurka->cestaFigurky[6][1] = 4;

            figurka->cestaFigurky[7][0] = 1;
            figurka->cestaFigurky[7][1] = 4;

            figurka->cestaFigurky[8][0] = 0;
            figurka->cestaFigurky[8][1] = 4;

            figurka->cestaFigurky[9][0] = 0;
            figurka->cestaFigurky[9][1] = 5;

            figurka->cestaFigurky[10][0] = 0;
            figurka->cestaFigurky[10][1] = 6;

            figurka->cestaFigurky[11][0] = 1;
            figurka->cestaFigurky[11][1] = 6;

            figurka->cestaFigurky[12][0] = 2;
            figurka->cestaFigurky[12][1] = 6;

            figurka->cestaFigurky[13][0] = 3;
            figurka->cestaFigurky[13][1] = 6;

            figurka->cestaFigurky[14][0] = 4;
            figurka->cestaFigurky[14][1] = 6;

            figurka->cestaFigurky[15][0] = 4;
            figurka->cestaFigurky[15][1] = 7;

            figurka->cestaFigurky[16][0] = 4;
            figurka->cestaFigurky[16][1] = 8;

            figurka->cestaFigurky[17][0] = 4;
            figurka->cestaFigurky[17][1] = 9;

            figurka->cestaFigurky[18][0] = 4;
            figurka->cestaFigurky[18][1] = 10;

            figurka->cestaFigurky[19][0] = 5;
            figurka->cestaFigurky[19][1] = 10;

            figurka->cestaFigurky[20][0] = 6;
            figurka->cestaFigurky[20][1] = 10;

            figurka->cestaFigurky[21][0] = 6;
            figurka->cestaFigurky[21][1] = 9;

            figurka->cestaFigurky[22][0] = 6;
            figurka->cestaFigurky[22][1] = 8;

            figurka->cestaFigurky[23][0] = 6;
            figurka->cestaFigurky[23][1] = 7;

            figurka->cestaFigurky[24][0] = 6;
            figurka->cestaFigurky[24][1] = 6;

            figurka->cestaFigurky[25][0] = 7;
            figurka->cestaFigurky[25][1] = 6;

            figurka->cestaFigurky[26][0] = 8;
            figurka->cestaFigurky[26][1] = 6;

            figurka->cestaFigurky[27][0] = 9;
            figurka->cestaFigurky[27][1] = 6;

            figurka->cestaFigurky[28][0] = 10;
            figurka->cestaFigurky[28][1] = 6;

            figurka->cestaFigurky[29][0] = 10;
            figurka->cestaFigurky[29][1] = 5;

            figurka->cestaFigurky[30][0] = 10;
            figurka->cestaFigurky[30][1] = 4;

            figurka->cestaFigurky[31][0] = 9;
            figurka->cestaFigurky[31][1] = 4;

            figurka->cestaFigurky[32][0] = 8;
            figurka->cestaFigurky[32][1] = 4;

            figurka->cestaFigurky[33][0] = 7;
            figurka->cestaFigurky[33][1] = 4;

            figurka->cestaFigurky[34][0] = 6;
            figurka->cestaFigurky[34][1] = 4;

            figurka->cestaFigurky[35][0] = 6;
            figurka->cestaFigurky[35][1] = 3;

            figurka->cestaFigurky[36][0] = 6;
            figurka->cestaFigurky[36][1] = 2;

            figurka->cestaFigurky[37][0] = 6;
            figurka->cestaFigurky[37][1] = 1;

            figurka->cestaFigurky[38][0] = 6;
            figurka->cestaFigurky[38][1] = 0;

            figurka->cestaFigurky[39][0] = 5;
            figurka->cestaFigurky[39][1] = 0;

            figurka->cestaFigurky[40][0] = 5;
            figurka->cestaFigurky[40][1] = 1;

            figurka->cestaFigurky[41][0] = 5;
            figurka->cestaFigurky[41][1] = 2;

            figurka->cestaFigurky[42][0] = 5;
            figurka->cestaFigurky[42][1] = 3;

            figurka->cestaFigurky[43][0] = 5;
            figurka->cestaFigurky[43][1] = 4;
            break;
        case Modra:
            figurka->cestaFigurky[0][0] = 0;
            figurka->cestaFigurky[0][1] = 6;

            figurka->cestaFigurky[1][0] = 1;
            figurka->cestaFigurky[1][1] = 6;

            figurka->cestaFigurky[2][0] = 2;
            figurka->cestaFigurky[2][1] = 6;

            figurka->cestaFigurky[3][0] = 3;
            figurka->cestaFigurky[3][1] = 6;

            figurka->cestaFigurky[4][0] = 4;
            figurka->cestaFigurky[4][1] = 6;

            figurka->cestaFigurky[5][0] = 4;
            figurka->cestaFigurky[5][1] = 7;

            figurka->cestaFigurky[6][0] = 4;
            figurka->cestaFigurky[6][1] = 8;

            figurka->cestaFigurky[7][0] = 4;
            figurka->cestaFigurky[7][1] = 9;

            figurka->cestaFigurky[8][0] = 4;
            figurka->cestaFigurky[8][1] = 10;

            figurka->cestaFigurky[9][0] = 5;
            figurka->cestaFigurky[9][1] = 10;

            figurka->cestaFigurky[10][0] = 6;
            figurka->cestaFigurky[10][1] = 10;

            figurka->cestaFigurky[11][0] = 6;
            figurka->cestaFigurky[11][1] = 9;

            figurka->cestaFigurky[12][0] = 6;
            figurka->cestaFigurky[12][1] = 8;

            figurka->cestaFigurky[13][0] = 6;
            figurka->cestaFigurky[13][1] = 7;

            figurka->cestaFigurky[14][0] = 6;
            figurka->cestaFigurky[14][1] = 6;

            figurka->cestaFigurky[15][0] = 7;
            figurka->cestaFigurky[15][1] = 6;

            figurka->cestaFigurky[16][0] = 8;
            figurka->cestaFigurky[16][1] = 6;

            figurka->cestaFigurky[17][0] = 9;
            figurka->cestaFigurky[17][1] = 6;

            figurka->cestaFigurky[18][0] = 10;
            figurka->cestaFigurky[18][1] = 6;

            figurka->cestaFigurky[19][0] = 10;
            figurka->cestaFigurky[19][1] = 5;

            figurka->cestaFigurky[20][0] = 10;
            figurka->cestaFigurky[20][1] = 4;

            figurka->cestaFigurky[21][0] = 9;
            figurka->cestaFigurky[21][1] = 4;

            figurka->cestaFigurky[22][0] = 8;
            figurka->cestaFigurky[22][1] = 4;

            figurka->cestaFigurky[23][0] = 7;
            figurka->cestaFigurky[23][1] = 4;

            figurka->cestaFigurky[24][0] = 6;
            figurka->cestaFigurky[24][1] = 4;

            figurka->cestaFigurky[25][0] = 6;
            figurka->cestaFigurky[25][1] = 3;

            figurka->cestaFigurky[26][0] = 6;
            figurka->cestaFigurky[26][1] = 2;

            figurka->cestaFigurky[27][0] = 6;
            figurka->cestaFigurky[27][1] = 1;

            figurka->cestaFigurky[28][0] = 6;
            figurka->cestaFigurky[28][1] = 0;

            figurka->cestaFigurky[29][0] = 5;
            figurka->cestaFigurky[29][1] = 0;

            figurka->cestaFigurky[30][0] = 4;
            figurka->cestaFigurky[30][1] = 0;

            figurka->cestaFigurky[31][0] = 4;
            figurka->cestaFigurky[31][1] = 1;

            figurka->cestaFigurky[32][0] = 4;
            figurka->cestaFigurky[32][1] = 2;

            figurka->cestaFigurky[33][0] = 4;
            figurka->cestaFigurky[33][1] = 3;

            figurka->cestaFigurky[34][0] = 4;
            figurka->cestaFigurky[34][1] = 4;

            figurka->cestaFigurky[35][0] = 3;
            figurka->cestaFigurky[35][1] = 4;

            figurka->cestaFigurky[36][0] = 2;
            figurka->cestaFigurky[36][1] = 4;

            figurka->cestaFigurky[37][0] = 1;
            figurka->cestaFigurky[37][1] = 4;

            figurka->cestaFigurky[38][0] = 0;
            figurka->cestaFigurky[38][1] = 4;

            figurka->cestaFigurky[39][0] = 0;
            figurka->cestaFigurky[39][1] = 5;

            figurka->cestaFigurky[40][0] = 1;
            figurka->cestaFigurky[40][1] = 5;

            figurka->cestaFigurky[41][0] = 2;
            figurka->cestaFigurky[41][1] = 5;

            figurka->cestaFigurky[42][0] = 3;
            figurka->cestaFigurky[42][1] = 5;

            figurka->cestaFigurky[43][0] = 4;
            figurka->cestaFigurky[43][1] = 5;
            break;
        case Zelena:
            figurka->cestaFigurky[0][0] = 6;
            figurka->cestaFigurky[0][1] = 10;

            figurka->cestaFigurky[1][0] = 6;
            figurka->cestaFigurky[1][1] = 9;

            figurka->cestaFigurky[2][0] = 6;
            figurka->cestaFigurky[2][1] = 8;

            figurka->cestaFigurky[3][0] = 6;
            figurka->cestaFigurky[3][1] = 7;

            figurka->cestaFigurky[4][0] = 6;
            figurka->cestaFigurky[4][1] = 6;

            figurka->cestaFigurky[5][0] = 7;
            figurka->cestaFigurky[5][1] = 6;

            figurka->cestaFigurky[6][0] = 8;
            figurka->cestaFigurky[6][1] = 6;

            figurka->cestaFigurky[7][0] = 9;
            figurka->cestaFigurky[7][1] = 6;

            figurka->cestaFigurky[8][0] = 10;
            figurka->cestaFigurky[8][1] = 6;

            figurka->cestaFigurky[9][0] = 10;
            figurka->cestaFigurky[9][1] = 5;

            figurka->cestaFigurky[10][0] = 10;
            figurka->cestaFigurky[10][1] = 4;

            figurka->cestaFigurky[11][0] = 9;
            figurka->cestaFigurky[11][1] = 4;

            figurka->cestaFigurky[12][0] = 8;
            figurka->cestaFigurky[12][1] = 4;

            figurka->cestaFigurky[13][0] = 7;
            figurka->cestaFigurky[13][1] = 4;

            figurka->cestaFigurky[14][0] = 6;
            figurka->cestaFigurky[14][1] = 4;

            figurka->cestaFigurky[15][0] = 6;
            figurka->cestaFigurky[15][1] = 3;

            figurka->cestaFigurky[16][0] = 6;
            figurka->cestaFigurky[16][1] = 2;

            figurka->cestaFigurky[17][0] = 6;
            figurka->cestaFigurky[17][1] = 1;

            figurka->cestaFigurky[18][0] = 6;
            figurka->cestaFigurky[18][1] = 0;

            figurka->cestaFigurky[19][0] = 5;
            figurka->cestaFigurky[19][1] = 0;

            figurka->cestaFigurky[20][0] = 4;
            figurka->cestaFigurky[20][1] = 0;

            figurka->cestaFigurky[21][0] = 4;
            figurka->cestaFigurky[21][1] = 1;

            figurka->cestaFigurky[22][0] = 4;
            figurka->cestaFigurky[22][1] = 2;

            figurka->cestaFigurky[23][0] = 4;
            figurka->cestaFigurky[23][1] = 3;

            figurka->cestaFigurky[24][0] = 4;
            figurka->cestaFigurky[24][1] = 4;

            figurka->cestaFigurky[25][0] = 3;
            figurka->cestaFigurky[25][1] = 4;

            figurka->cestaFigurky[26][0] = 2;
            figurka->cestaFigurky[26][1] = 4;

            figurka->cestaFigurky[27][0] = 1;
            figurka->cestaFigurky[27][1] = 4;

            figurka->cestaFigurky[28][0] = 0;
            figurka->cestaFigurky[28][1] = 4;

            figurka->cestaFigurky[29][0] = 0;
            figurka->cestaFigurky[29][1] = 5;

            figurka->cestaFigurky[30][0] = 0;
            figurka->cestaFigurky[30][1] = 6;

            figurka->cestaFigurky[31][0] = 1;
            figurka->cestaFigurky[31][1] = 6;

            figurka->cestaFigurky[32][0] = 2;
            figurka->cestaFigurky[32][1] = 6;

            figurka->cestaFigurky[33][0] = 3;
            figurka->cestaFigurky[33][1] = 6;

            figurka->cestaFigurky[34][0] = 4;
            figurka->cestaFigurky[34][1] = 6;

            figurka->cestaFigurky[35][0] = 4;
            figurka->cestaFigurky[35][1] = 7;

            figurka->cestaFigurky[36][0] = 4;
            figurka->cestaFigurky[36][1] = 8;

            figurka->cestaFigurky[37][0] = 4;
            figurka->cestaFigurky[37][1] = 9;

            figurka->cestaFigurky[38][0] = 4;
            figurka->cestaFigurky[38][1] = 10;

            figurka->cestaFigurky[39][0] = 5;
            figurka->cestaFigurky[39][1] = 10;

            figurka->cestaFigurky[40][0] = 5;
            figurka->cestaFigurky[40][1] = 9;

            figurka->cestaFigurky[41][0] = 5;
            figurka->cestaFigurky[41][1] = 8;

            figurka->cestaFigurky[42][0] = 5;
            figurka->cestaFigurky[42][1] = 7;

            figurka->cestaFigurky[43][0] = 5;
            figurka->cestaFigurky[43][1] = 6;
            break;
        case Zlta:
            figurka->cestaFigurky[0][0] = 10;
            figurka->cestaFigurky[0][1] = 4;

            figurka->cestaFigurky[1][0] = 9;
            figurka->cestaFigurky[1][1] = 4;

            figurka->cestaFigurky[2][0] = 8;
            figurka->cestaFigurky[2][1] = 4;

            figurka->cestaFigurky[3][0] = 7;
            figurka->cestaFigurky[3][1] = 4;

            figurka->cestaFigurky[4][0] = 6;
            figurka->cestaFigurky[4][1] = 4;

            figurka->cestaFigurky[5][0] = 6;
            figurka->cestaFigurky[5][1] = 3;

            figurka->cestaFigurky[6][0] = 6;
            figurka->cestaFigurky[6][1] = 2;

            figurka->cestaFigurky[7][0] = 6;
            figurka->cestaFigurky[7][1] = 1;

            figurka->cestaFigurky[8][0] = 6;
            figurka->cestaFigurky[8][1] = 0;

            figurka->cestaFigurky[9][0] = 5;
            figurka->cestaFigurky[9][1] = 0;

            figurka->cestaFigurky[10][0] = 4;
            figurka->cestaFigurky[10][1] = 0;

            figurka->cestaFigurky[11][0] = 4;
            figurka->cestaFigurky[11][1] = 1;

            figurka->cestaFigurky[12][0] = 4;
            figurka->cestaFigurky[12][1] = 2;

            figurka->cestaFigurky[13][0] = 4;
            figurka->cestaFigurky[13][1] = 3;

            figurka->cestaFigurky[14][0] = 4;
            figurka->cestaFigurky[14][1] = 4;

            figurka->cestaFigurky[15][0] = 3;
            figurka->cestaFigurky[15][1] = 4;

            figurka->cestaFigurky[16][0] = 2;
            figurka->cestaFigurky[16][1] = 4;

            figurka->cestaFigurky[17][0] = 1;
            figurka->cestaFigurky[17][1] = 4;

            figurka->cestaFigurky[18][0] = 0;
            figurka->cestaFigurky[18][1] = 4;

            figurka->cestaFigurky[19][0] = 0;
            figurka->cestaFigurky[19][1] = 5;

            figurka->cestaFigurky[20][0] = 0;
            figurka->cestaFigurky[20][1] = 6;

            figurka->cestaFigurky[21][0] = 1;
            figurka->cestaFigurky[21][1] = 6;

            figurka->cestaFigurky[22][0] = 2;
            figurka->cestaFigurky[22][1] = 6;

            figurka->cestaFigurky[23][0] = 3;
            figurka->cestaFigurky[23][1] = 6;

            figurka->cestaFigurky[24][0] = 4;
            figurka->cestaFigurky[24][1] = 6;

            figurka->cestaFigurky[25][0] = 4;
            figurka->cestaFigurky[25][1] = 7;

            figurka->cestaFigurky[26][0] = 4;
            figurka->cestaFigurky[26][1] = 8;

            figurka->cestaFigurky[27][0] = 4;
            figurka->cestaFigurky[27][1] = 9;

            figurka->cestaFigurky[28][0] = 4;
            figurka->cestaFigurky[28][1] = 10;

            figurka->cestaFigurky[29][0] = 5;
            figurka->cestaFigurky[29][1] = 10;

            figurka->cestaFigurky[30][0] = 6;
            figurka->cestaFigurky[30][1] = 10;

            figurka->cestaFigurky[31][0] = 6;
            figurka->cestaFigurky[31][1] = 9;

            figurka->cestaFigurky[32][0] = 6;
            figurka->cestaFigurky[32][1] = 8;

            figurka->cestaFigurky[33][0] = 6;
            figurka->cestaFigurky[33][1] = 7;

            figurka->cestaFigurky[34][0] = 6;
            figurka->cestaFigurky[34][1] = 6;

            figurka->cestaFigurky[35][0] = 7;
            figurka->cestaFigurky[35][1] = 6;

            figurka->cestaFigurky[36][0] = 8;
            figurka->cestaFigurky[36][1] = 6;

            figurka->cestaFigurky[37][0] = 9;
            figurka->cestaFigurky[37][1] = 6;

            figurka->cestaFigurky[38][0] = 10;
            figurka->cestaFigurky[38][1] = 6;

            figurka->cestaFigurky[39][0] = 10;
            figurka->cestaFigurky[39][1] = 5;

            figurka->cestaFigurky[40][0] = 9;
            figurka->cestaFigurky[40][1] = 5;

            figurka->cestaFigurky[41][0] = 8;
            figurka->cestaFigurky[41][1] = 5;

            figurka->cestaFigurky[42][0] = 7;
            figurka->cestaFigurky[42][1] = 5;

            figurka->cestaFigurky[43][0] = 6;
            figurka->cestaFigurky[43][1] = 5;
            break;
        default:
            break;
    }
}

void presunFigurku(FIGURKA* figurka, int oKolko, int figurkaID) {
    int tempPocetPrejdenychPolicok;
    tempPocetPrejdenychPolicok = figurka->pocetPrejdenychPolicok;
    figurka->poziciaRiadok = figurka->cestaFigurky[tempPocetPrejdenychPolicok + oKolko][0];
    figurka->poziciaStlpec = figurka->cestaFigurky[tempPocetPrejdenychPolicok + oKolko][1];
    figurka->pocetPrejdenychPolicok += oKolko;
}

char* dajFarbuHraca(HRAC* hrac) {
    char* farbaHracaText;
    switch (hrac->farbaHraca) {
        case Cervena:
            farbaHracaText = "CERVENA";
            break;
        case Modra:
            farbaHracaText = "MODRA";
            break;
        case Zelena:
            farbaHracaText = "ZELENA";
            break;
        case Zlta:
            farbaHracaText = "ZLTA";
            break;
        default:
            break;
    }
    return farbaHracaText;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    /*-------------------------------------------- INICIALIZACIA HRACOV --------------------------------------------*/
    int pocetHracov;
    if (argc > 1) {
        pocetHracov = atoi(argv[1]);
    } else {
        pocetHracov = 4;
    }

    if (pocetHracov == 1) {
        printf("Pocet hracov musi byt vacsi ako 1!\n");
        return 1;
    }

    HRAC* hraci = (HRAC*)malloc(pocetHracov * sizeof(HRAC));

    char* zapisaniHraci[pocetHracov];
    for(int i = 0; i < pocetHracov; i++) {
        zapisaniHraci[i] = (char*)calloc(20, sizeof(char));
    }

    for(int i = 0; i < pocetHracov; i++) {
        printf("Zadajte meno %d. hraca: ", i + 1);
        scanf("%19s", hraci[i].meno);
        for (int j = 0; j < pocetHracov; ++j) {
            while (strcmp(hraci[i].meno, zapisaniHraci[j]) == 0) {
                printf("Zadane meno uz existuje...\n");
                j = 0;
                printf("Zadajte meno %d hraca: ", i + 1);
                scanf("%19s", hraci[i].meno);
            }
        }
        strcpy(zapisaniHraci[i], hraci[i].meno);
    }

    for (int i = 0; i < pocetHracov; ++i) {
        free(zapisaniHraci[i]);
    }

    switch (pocetHracov) {
        case 2:
            hraci[0].farbaHraca = Cervena;
            hraci[1].farbaHraca = Modra;
            break;
        case 3:
            hraci[0].farbaHraca = Cervena;
            hraci[1].farbaHraca = Modra;
            hraci[2].farbaHraca = Zelena;
            break;
        case 4:
            hraci[0].farbaHraca = Cervena;
            hraci[1].farbaHraca = Modra;
            hraci[2].farbaHraca = Zelena;
            hraci[3].farbaHraca = Zlta;
            break;
        default:
            break;
    }

    for (int i = 0; i < pocetHracov; ++i) {
        hraci[i].id = i + 1;
        hraci[i].pocetFiguriek = 4;
        hraci[i].pocetFiguriekVCieli = 0;

        FIGURKA* figurkyHraca = (FIGURKA*)malloc(4 * sizeof(FIGURKA));
        for (int j = 0; j < 4; ++j) {
            figurkyHraca[j].poziciaRiadok = -1;
            figurkyHraca[j].poziciaStlpec = -1;
            priradCestuFigurke(&figurkyHraca[j], hraci[i].farbaHraca);
            figurkyHraca[j].figurkaID = j + 1;
            figurkyHraca[j].pocetPrejdenychPolicok = -1;
        }
        hraci[i].figurkyHraca = figurkyHraca;
    }

    /*----------------------------------------- INICIALIZACIA HRACEJ PLOCHY -----------------------------------------*/
    /**
     * Hracia plocha, reprezentovna 2D maticou 11x11 policok
     * Co budeme potrebovat:
     *      - do kazdeho rohu 2D maticu 4x4 prazdnych policok:
     *          - od hraciaPlocha[0][0] po hraciaPlocha[3][3]
     *          - od hraciaPlocha[0][7] po hraciaPlocha[3][10]
     *          - od hraciaPlocha[7][0] po hraciaPlocha[10][3]
     *          - od hraciaPlocha[7][7] po hraciaPlocha[10][10]
     *
     *     - na poziciu hraciaPlocha[5][5] prazdne
     *
     *      - dvakrat 2D maticu 3x3 vertikalnych policok:
     *          - od hraciaPlocha[1][4] po hraciaPlocha[3][6]
     *          - od hraciaPlocha[7][4] po hraciaPlocha[9][6]
     *
     *      - dvakrat 2D maticu 3x3 horizontalnych policok:
     *          - od hraciaPlocha[4][1] po hraciaPlocha[6][3]
     *          - od hraciaPlocha[4][7] po hraciaPlocha[6][9]
     *
     *      - na poziciu hraciaPlocha[4][0] ZdolaDoprava
     *      - na poziciu hraciaPlocha[5][0] DomovVpravo
     *      - na poziciu hraciaPlocha[6][0] ZhoraDoprava
     *
     *      - na poziciu hraciaPlocha[0][4] ZdolaDoprava
     *      - na poziciu hraciaPlocha[0][5] DomovDole
     *      - na poziciu hraciaPlocha[0][6] ZlavaDole
     *
     *      - na poziciu hraciaPlocha[4][10] ZlavaDole
     *      - na poziciu hraciaPlocha[5][10] DomovVlavo
     *      - na poziciu hraciaPlocha[6][10] ZlavaHore
     *
     *      - na poziciu hraciaPlocha[10][4] ZhoraDoprava
     *      - na poziciu hraciaPlocha[10][5] DomovHore
     *      - na poziciu hraciaPlocha[10][6] ZlavaHore
     *
     *      - na poziciu hraciaPlocha[4][4] ZlavaHore
     *      - na poziciu hraciaPlocha[6][4] ZlavaDole
     *      - na poziciu hraciaPlocha[4][6] ZhoraDoprava
     *      - na poziciu hraciaPlocha[6][6] ZdolaDoprava
     *
     *      - na poziciu hraciaPlocha[4][5] DomovKoniecDole
     *      - na poziciu hraciaPlocha[5][4] DomovKoniecVpravo
     *      - na poziciu hraciaPlocha[5][6] DomovKoniecVlavo
     *      - na poziciu hraciaPlocha[6][5] DomovKoniecHore
     * */
    POLICKO hraciaPlocha[11][11];
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            for (int k = 0; k < 3; ++k) {
                for (int m = 0; m < 3; ++m) {
                    hraciaPlocha[i][j].obsahPolicka[k][m] = '?';
                }
            }
            hraciaPlocha[i][j].typPolicka = Neinicializovane;
            hraciaPlocha[i][j].jeNaMneHrac = 0;
        }
    }

    /**
     * Netreba dookola aktualizovat obsah "prazdnych" policok, len na zaciatku hry
     * */
    nastavHraciuPlochuPRAZDE(hraciaPlocha);
    nastavHraciuPlochu(hraciaPlocha);

    /*---------- ZACIATOK HRY ----------*/

    int nahodyHracID = dajNahodneCisloVRozsahu(1,pocetHracov);
    HRAC* hracNaTahu = dajHraca(hraci, pocetHracov, nahodyHracID);
    printf("Zacina hrac: %s\n", hracNaTahu->meno);
    sleep(1);
    nastavHraciuPlochuPRAZDE(hraciaPlocha);
    int koniecHry = 0;
    int naTahuID = nahodyHracID;
    int hodKockou;

    char inputHry = '\0';               // NULL char alebo nejake cislo, ak je to cislo, konvertuje sa na INT
    int inputHryPoradoveCisloFigurky;   // hrac moze mat na hracom poli viacero figuriek a moze si vybrat s ktorou pohne

    int poziciaRiadokHraciaPlocha;
    int poziciaStlpecHraciaPlocha;

    nastavHraciuPlochu(hraciaPlocha);

    while (koniecHry == 0) {
        vycistiKonzolu();
        vykresliHraciuPlochu(hraciaPlocha);
        hracNaTahu = dajHraca(hraci, pocetHracov, naTahuID);

        switch (skontrolujStavHraca(hracNaTahu)) {
            case 1:
                koniecHry = 1;
                break;
            case 2:
                printf("Na tahu: %s\n", hracNaTahu->meno);
                printf("Farba hraca: %s\n", dajFarbuHraca(hracNaTahu));
                printf("Hrac %s nema na hracej ploche ziadnu figurku!\n", hracNaTahu->meno);
                for (int i = 0; i < 3; i++) {
                    printf("Pre hod stlacte ENTER...");
                    scanf("%c", &inputHry);
                    if (inputHry != '\0' && isdigit(inputHry)) {
                        inputHryPoradoveCisloFigurky = atoi(&inputHry);
                        if (inputHryPoradoveCisloFigurky < 1 || inputHryPoradoveCisloFigurky > 4) {
                            inputHryPoradoveCisloFigurky = 1;
                        }
                    } else {
                        inputHryPoradoveCisloFigurky = 1;
                    }

                    hodKockou = dajNahodneCisloVRozsahu(1,6);
                    printf("Hrac %s hadze kockou: %d\n", hracNaTahu->meno, hodKockou);
                    sleep(1);
                    if (hodKockou == 6) {
                        printf("Hrac %s hodil 6! Presuva figurku na startovacie policko!\n", hracNaTahu->meno);

                        FIGURKA* posuvanaFigurka = NULL;
                        for (int j = 0; j < hracNaTahu->pocetFiguriek; j++) {
                            if (hracNaTahu->figurkyHraca[j].figurkaID == inputHryPoradoveCisloFigurky) {
                                posuvanaFigurka = &(hracNaTahu->figurkyHraca[j]);
                                break;
                            }
                        }

                        presunFigurku(posuvanaFigurka, 1, posuvanaFigurka->figurkaID);
                        poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                        poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                        nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca);
                        break;
                    }
                }

                naTahuID++;
                if (naTahuID > pocetHracov) {
                    naTahuID = 1;
                }
                break;
            case 3:
                printf("Na tahu: %s\n", hracNaTahu->meno);
                printf("Farba hraca: %s\n", dajFarbuHraca(hracNaTahu));
                printf("Hrac %s hadze kockou!\n", hracNaTahu->meno);
                printf("Pre hod stlacte ENTER...");
                scanf("");
                hodKockou = dajNahodneCisloVRozsahu(1,6);
                printf("Hrac %s hadze kockou: %d\n", hracNaTahu->meno, hodKockou);
                if (hodKockou == 6) {
                    int maVolneMiesto = 1;
                    for (int i = 0; i < hracNaTahu->pocetFiguriek; ++i) {
                        if  (hracNaTahu->figurkyHraca[i].pocetPrejdenychPolicok == 0) {
                            printf("Hrac ma na startovacej pozicii figurku. Nemoze pridat dalsiu.");
                            sleep(1);
                            maVolneMiesto = 0;
                            break;
                        }
                    }
                    if (maVolneMiesto) {
                        printf("Hrac pridava na hraciu plochu dalsiu figurku...");
                        scanf("%c", &inputHry);
                        if (inputHry != '\0' && isdigit(inputHry)) {
                            inputHryPoradoveCisloFigurky = atoi(&inputHry);
                            if (inputHryPoradoveCisloFigurky < 1 || inputHryPoradoveCisloFigurky > 4) {
                                inputHryPoradoveCisloFigurky = 1;
                            }
                        } else {
                            inputHryPoradoveCisloFigurky = 1;
                        }

                        FIGURKA* posuvanaFigurka = NULL;
                        for (int j = 0; j < hracNaTahu->pocetFiguriek; j++) {
                            if (hracNaTahu->figurkyHraca[j].figurkaID == inputHryPoradoveCisloFigurky) {
                                posuvanaFigurka = &(hracNaTahu->figurkyHraca[j]);
                                break;
                            }
                        }

                        presunFigurku(posuvanaFigurka, 1, posuvanaFigurka->figurkaID);
                        poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                        poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                        nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca);
                        break;
                    }
                } else {
                    printf("Hrac posuva figurku...");
                    scanf("%c", &inputHry);
                    if (inputHry != '\0' && isdigit(inputHry)) {
                        inputHryPoradoveCisloFigurky = atoi(&inputHry);
                        if (inputHryPoradoveCisloFigurky < 1 || inputHryPoradoveCisloFigurky > 4) {
                            inputHryPoradoveCisloFigurky = 1;
                        }
                    } else {
                        inputHryPoradoveCisloFigurky = 1;
                    }

                    FIGURKA* posuvanaFigurka = NULL;
                    for (int j = 0; j < hracNaTahu->pocetFiguriek; j++) {
                        if (hracNaTahu->figurkyHraca[j].figurkaID == inputHryPoradoveCisloFigurky) {
                            posuvanaFigurka = &(hracNaTahu->figurkyHraca[j]);
                            break;
                        }
                    }

                    vyprazdniPolicko(&hraciaPlocha[posuvanaFigurka->poziciaRiadok][posuvanaFigurka->poziciaStlpec]);
                    presunFigurku(posuvanaFigurka, hodKockou, posuvanaFigurka->figurkaID);
                    poziciaRiadokHraciaPlocha = posuvanaFigurka->poziciaRiadok;
                    poziciaStlpecHraciaPlocha = posuvanaFigurka->poziciaStlpec;
                    nastavObsahPolickaFigurka(&hraciaPlocha[poziciaRiadokHraciaPlocha][poziciaStlpecHraciaPlocha], hracNaTahu->farbaHraca);
                }

                naTahuID++;
                if (naTahuID > pocetHracov) {
                    naTahuID = 1;
                }
                break;
            default:
                printf("default vetva\n");
                break;
        }
    }

    printf("KONIEC HRY!\n");
    printf("Zvitazil hrac: %s", hracNaTahu->meno);

    /*----------------------------------------- UVOLNOVANIE PAMATE -----------------------------------------*/
    for (int i = 0; i < pocetHracov; ++i) {
        free(hraci[i].figurkyHraca);
    }
    free(hraci);
    return 0;
}