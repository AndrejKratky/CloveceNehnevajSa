#include "hra.h"

void inicializaciaHracejPlochy(POLICKO* hraciaPlocha[11]) {
    for (int i = 0; i < 11; ++i) {
        hraciaPlocha[i] = (POLICKO*)malloc(11 * sizeof(POLICKO));
    }

    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            for (int k = 0; k < 3; ++k) {
                for (int m = 0; m < 3; ++m) {
                    hraciaPlocha[i][j].obsahPolicka[k][m] = '?';
                }
            }
            hraciaPlocha[i][j].jeNaMneHrac = 0;
            hraciaPlocha[i][j].figurkaHraca = NULL;
        }
    }
}

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
    char* riadokPolicka = (char*)calloc(4, sizeof(char));
    for (int i = 0; i < 3; i++) {
        riadokPolicka[i] = policko->obsahPolicka[riadok - 1][i];
    }
    riadokPolicka[3] = '\0';
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

    } else if (p_typPolicka == Vertikalne) {
        for (int i = 0; i < 3; ++i) {
            policko->obsahPolicka[i][0] = '|';
            policko->obsahPolicka[i][2] = '|';
        }
        for (int i = 0; i < 3; ++i) {
            policko->obsahPolicka[i][1] = ' ';
        }

    } else if (p_typPolicka == Prazdne) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                policko->obsahPolicka[i][j] = 'X';
            }
        }

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

    } else if (p_typPolicka == DomovHore) {
        policko->obsahPolicka[0][0] = '_';
        policko->obsahPolicka[0][1] = '_';
        policko->obsahPolicka[0][2] = '_';

        policko->obsahPolicka[1][0] = ' ';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = ' ';

        policko->obsahPolicka[2][0] = '-';
        policko->obsahPolicka[2][1] = '-';
        policko->obsahPolicka[2][2] = '-';

    } else if (p_typPolicka == DomovVpravo) {
        policko->obsahPolicka[0][0] = '|';
        policko->obsahPolicka[0][1] = ' ';
        policko->obsahPolicka[0][2] = '|';

        policko->obsahPolicka[1][0] = '|';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = '|';

        policko->obsahPolicka[2][0] = '|';
        policko->obsahPolicka[2][1] = ' ';
        policko->obsahPolicka[2][2] = '|';

    } else if (p_typPolicka == DomovDole) {
        policko->obsahPolicka[0][0] = '-';
        policko->obsahPolicka[0][1] = '-';
        policko->obsahPolicka[0][2] = '-';

        policko->obsahPolicka[1][0] = ' ';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = ' ';

        policko->obsahPolicka[2][0] = '-';
        policko->obsahPolicka[2][1] = '-';
        policko->obsahPolicka[2][2] = '-';

    } else if (p_typPolicka == DomovVlavo) {
        policko->obsahPolicka[0][0] = '|';
        policko->obsahPolicka[0][1] = ' ';
        policko->obsahPolicka[0][2] = '|';

        policko->obsahPolicka[1][0] = '|';
        policko->obsahPolicka[1][1] = ' ';
        policko->obsahPolicka[1][2] = '|';

        policko->obsahPolicka[2][0] = '|';
        policko->obsahPolicka[2][1] = ' ';
        policko->obsahPolicka[2][2] = '|';

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

    } else {
        return;
    }
}

void nastavObsahPolickaFigurka(POLICKO* policko, FARBA p_farba, FIGURKA* p_presuvanaFigurka) {
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
    policko->jeNaMneHrac = 0;
    policko->figurkaHraca = NULL;
}

/**
 * Stav prazdnych policok sa nemusi aktualizovat, kedze na ne figurka nemoze skocit.
 * Tato funkcia sa zavola len raz v celom programe.
 * */
void nastavHraciuPlochuPRAZDE(POLICKO* p_hraciaPlocha[11]) {
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
void nastavHraciuPlochu(POLICKO* p_hraciaPlocha[11]) {
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

void vykresliHraciuPlochu(POLICKO* p_hraciaPlocha[11]) {
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

void priradCestuFigurke(FIGURKA* figurka, FARBA farba) {
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

int presunFigurku(FIGURKA* figurka, int oKolko, int figurkaID, POLICKO* hraciaPlocha[11]) {
    int tempPocetPrejdenychPolicok;
    tempPocetPrejdenychPolicok = figurka->pocetPrejdenychPolicok;

    if (figurka->pocetPrejdenychPolicok + oKolko > 43) {
        return 4;   // je nemozne posunut sa o tolko policok s figurkou
    }

    figurka->poziciaRiadok = figurka->cestaFigurky[tempPocetPrejdenychPolicok + oKolko][0];
    figurka->poziciaStlpec = figurka->cestaFigurky[tempPocetPrejdenychPolicok + oKolko][1];
    figurka->pocetPrejdenychPolicok += oKolko;

    POLICKO* cielovePolicko = &hraciaPlocha[figurka->poziciaRiadok][figurka->poziciaStlpec];

    FIGURKA* vyhadzovanaFigurka = NULL;
    if (cielovePolicko->jeNaMneHrac == 1) {
        vyhadzovanaFigurka = cielovePolicko->figurkaHraca;
        vyhadzovanaFigurka->pocetPrejdenychPolicok = -1;
        vyhadzovanaFigurka->poziciaRiadok = -1;
        vyhadzovanaFigurka->poziciaStlpec = -1;
        if (vyhadzovanaFigurka->farbaFigurky == figurka->farbaFigurky) {
            return 3;   // vyhodenie vlastnej figurky
        } else {
            return 2;   // vyhodenie nepriatelskej figurky
        }
    }
    cielovePolicko->jeNaMneHrac = 1;
    cielovePolicko->figurkaHraca = figurka;
    return 1;       // posunutie figurky
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

int jeNaHracejPloche(HRAC* hrac, int idFigurky) {
    for (int i = 0; i < hrac->pocetFiguriek; ++i) {
        if (hrac->figurkyHraca[i].figurkaID == idFigurky && hrac->figurkyHraca[i].pocetPrejdenychPolicok > -1) {
            return 1;
        }
    }
    return 0;
}

int dajPocetFigurokNaHracejPloche(HRAC* hrac) {
    int pocitadlo = 0;
    for (int i = 0; i < hrac->pocetFiguriek; ++i) {
        if (hrac->figurkyHraca[i].pocetPrejdenychPolicok > -1) {
            pocitadlo++;
        }
    }
    return pocitadlo;
}

int skontrolujStavHraca(HRAC* hrac) {
    int pocitadloVCieli = 0;
    for (int i = 0; i < hrac->pocetFiguriek; ++i) {
        if (hrac->figurkyHraca[i].pocetPrejdenychPolicok > 39) {
            pocitadloVCieli++;
        }
    }
    if (pocitadloVCieli == hrac->pocetFiguriek) {
        return 1;   // hrac vyhral
    }

    if (dajPocetFigurokNaHracejPloche(hrac) == 0) {
        return 2;   // hrac nema ziadnu figurku na hracej ploche
    }
    return 3;       // hrac ma aspon jednu figurku na hracej ploche
}
