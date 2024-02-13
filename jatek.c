#include <stdio.h>
#include <stdlib.h>

//#include "debugmalloc.h"
#include "jatek.h"
#include "grafika.h"

/* kétdimenziós tömb foglalás, adott x méret alapján
   visszaadja a lefoglalt tomb cimet */
int** foglal(int meret_x) {
    int **tomb = (int**) malloc(meret_x * sizeof(int*));
    for (int i = 0; i < meret_x; ++i) {
        tomb[i] = (int*) malloc(meret_x * sizeof(int));
    }
    return tomb;
}

/* kétdimenziós tömb felszabadítása
   meg kell adni a felszabaditando tombot es annak meretet */
void felszabadit(int** tomb, int meret_x) {
    for (int i = 0; i < meret_x; ++i) {
        free(tomb[i]);
    }
    free(tomb);
}

/* kezdőállapot tömb létrehozása, üres tomb, nullakkal van feltoltve */
int** empty(int meret_x) {
    //memóriafoglalás a kétdimenziós tömbnek
    int **tabla_start = foglal(meret_x);

    //tömb feltöltése nullákkal
    int i, j;
    for (i = 0; i < meret_x; ++i) {
        for (j = 0; j < meret_x; ++j) {
            tabla_start[i][j] = 0;
        }
    }

    return tabla_start;
}

/* megszámolja, hogy hány élő szomszédja van egy adott cellának
   i és j a cella pozciója a **tabla tömbben */
int szomszedok_szama(int i, int j, int **tabla) {
    int elo_szomszed = 0;
    for (int k = i - 1; k <= i + 1; ++k) {
        for (int l = j - 1; l <= j + 1; ++l) {
            if (tabla[k][l] == 1)
                elo_szomszed++;
        }
    }
    if (tabla[i][j] == 1) {
        elo_szomszed--;
    }
    return elo_szomszed;
}

//megkapott tömb alapján létrehozza a játéktér következő állapotát
int** next_state(int meret_x, int** previous_state) {
    int **next_state = foglal(meret_x);

    for (int i = 1; i < meret_x - 1; ++i) {
        for (int j = 1; j < meret_x - 1; ++j) {
            if (previous_state[i][j] == 1) {
                if (szomszedok_szama(i, j, previous_state) == 2 || szomszedok_szama(i, j, previous_state) == 3)
                    next_state[i][j] = 1;
                else
                    next_state[i][j] = 0;
            }
            else if (previous_state[i][j] == 0) {
                if (szomszedok_szama(i, j, previous_state) == 3)
                    next_state[i][j] = 1;
                else
                    next_state[i][j] = 0;
            }
        }
    }
    //felszabadit(next_state, meret_x, meret_y);
    return next_state;
}

//fájlban tárolt játékallapotot tolt be
int** input(int meret_x) {
    FILE *in;

    //fájl nyitása olvasásra
    in = fopen("mentes.txt", "r");
    if (in == NULL) {
        printf("Hiba, nem lehet beolvasni a fajlt.\n");
        return NULL;
    }

    int x;
    fscanf(in, "%d", &x);
    //printf("%d", x);

    //ellenorzes, hogy a belefer-e a jatekterbe a tolteni kivant minta
    //ha nem, akkor hibauzenetet kuld es nem tolti be a fajlt
    if(x > meret_x) {
        printf("A jatekter tul kicsi a betolteni kivant mintanak.\n");
        int** hiba = foglal(meret_x + 2);
        return hiba;
    }

    int** input = foglal(x);

    //beolvasás
    char sor[x];
    for (int i = 0; i < x; ++i) {
        fscanf(in, "%s", sor);
        for (int j = 0; j < x; ++j) {
            input[i][j] = sor[j] - 48;
        }
    }

    //helyes meret beallitasa, hogy a betoltes altal ne valtozzon a jatekter merete
    int** belerak = empty(meret_x);
       for(int i = 0; i < x; ++i) {
        for(int j = 0; j < x; ++j) {
            belerak[i + ((meret_x - x) / 2)][j + ((meret_x - x) / 2)] = input[i][j];
        }
    }

    //nullak kore
    int** kesz = empty(meret_x + 2);
    for (int i = 1; i < meret_x + 1; ++i) {
        for (int j = 1; j < meret_x + 1; ++j) {
            kesz[i][j] = belerak[i-1][j-1];
        }
    }

    felszabadit(belerak, meret_x);
    felszabadit(input, x);
    fclose(in);

    return kesz;
}

//fajlba mentest vegzo fuggveny
void ment(int meret_x, int** tomb) {
    FILE *out;

    //fajl nyitas
    out = fopen("mentes.txt", "w");
    if (out == NULL) {
        printf("Hiba, a fájl nem nyithato meg.\n");
        return NULL;
    }

    //meret beirasa
    fprintf(out, "%d\n", meret_x);

    //adatok beirasa
    for(int i = 1; i <= meret_x; ++i) {
        for(int j = 1; j <= meret_x; ++j){
            //printf("%d ", tomb[j][i]);
            fprintf(out, "%d", tomb[j][i]);
        }
        fprintf(out, "\n");
    }

    fclose(out);
}

//meretezesnel a minta megtartasahoz szukseges fuggveny
int** illeszt(int meret_x, int** tomb, int uj_meret_x) {
    int** igazitott = empty(uj_meret_x + 2);

    //felfele meretezes: a regi, kisebb tombot az uj tomb kozepebe masolja
    if (uj_meret_x > meret_x) {
        for(int i = 1; i < meret_x + 1; ++i) {
            for(int j = 1; j < meret_x + 1; ++j) {
                igazitott[i + ((uj_meret_x - meret_x) / 2)][j + ((uj_meret_x - meret_x) / 2)] = tomb[i][j];
            }
        }
    }
    //lefele meretezes: a regi, nagyobb tomb szeleit levagja, hogy a maradek beleferjen az uj, kisebb tombbe
    else if (uj_meret_x < meret_x) {
        for(int i = 1; i < uj_meret_x + 1; ++i) {
            for(int j = 1; j < uj_meret_x + 1; ++j) {
                igazitott[i][j] = tomb[i + ((meret_x - uj_meret_x) / 2)][j + ((meret_x - uj_meret_x) / 2)];
            }
        }
    }
    felszabadit(tomb, meret_x + 2);

    return igazitott;
}
