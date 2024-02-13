#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdio.h>

#include "jatek.h"
#include "grafika.h"
//#include "debugmalloc.h"

//sdl inicializalasa
void SDLInit(SDL_Window **pwindow, SDL_Renderer **prenderer) {
    /* SDL inicializálása ablak es renderer letrehozasa - infocrol masolva*/
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }

    SDL_Window *window = SDL_CreateWindow("Conway's Game of Life (by Mayer Adam)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1100, 800, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }

    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

/* rajzok*/
void elrendezes(SDL_Renderer *renderer) {
    /*jatekter elvalasztovonal*/
    lineColor(renderer, 800, 0, 800, 800, 0xFFFFFFFF);

    /*counter doboza*/
    rectangleColor(renderer, 820, 20, 1080, 180, 0xFFFFFFFF);

    /*jatekter manipulacija - bal klikk*/
    rectangleColor(renderer, 820, 200, 940, 280, 0xFFFFFFFF);
    stringColor(renderer, 865, 230, "Draw", 0xFFFFFFFF);
    stringColor(renderer, 833, 250, "[Left click]", 0xFFFFFFFF);

    /*jatekter manipulacioja - jobb klikk*/
    rectangleColor(renderer, 960, 200, 1080, 280, 0xFFFFFFFF);
    stringColor(renderer, 1002, 230, "Erase", 0xFFFFFFFF);
    stringColor(renderer, 967, 250, "[Right click]", 0xFFFFFFFF);

    /*kisebb jatekter gomb*/
    rectangleColor(renderer, 960, 300, 1080, 380, 0xFFFFFFFF);
    stringColor(renderer, 990, 330, "Zoom in", 0xFFFFFFFF);
    stringColor(renderer, 995, 350, "[Down]", 0xFFFFFFFF);

    /*nagyobb jatekter gomb*/
    rectangleColor(renderer, 820, 300, 940, 380, 0xFFFFFFFF);
    stringColor(renderer, 850, 330, "Zoom out", 0xFFFFFFFF);
    stringColor(renderer, 865, 350, "[Up]", 0xFFFFFFFF);

    /*leptet gomb*/
    rectangleColor(renderer, 820, 400, 1080, 480, 0xFFFFFFFF);
    stringColor(renderer, 930, 430, "Step", 0xFFFFFFFF);
    stringColor(renderer, 920, 450, "[Right]", 0xFFFFFFFF);

    /*pause gomb*/
    rectangleColor(renderer, 820, 500, 1080, 580, 0xFFFFFFFF);
    stringColor(renderer, 900, 530, "Pause / Play", 0xFFFFFFFF);
    stringColor(renderer, 920, 550, "[Space]", 0xFFFFFFFF);

    /*reset gomb*/
    rectangleColor(renderer, 820, 600, 1080, 680, 0xFFFFFFFF);
    stringColor(renderer, 920, 640, "[R]eset", 0xFFFFFFFF);

    /*load gomb*/
    rectangleColor(renderer, 820, 700, 940, 780, 0xFFFFFFFF);
    stringColor(renderer, 855, 740, "[L]oad", 0xFFFFFFFF);

    /*mentés gomb*/
    rectangleColor(renderer, 960, 700, 1080, 780, 0xFFFFFFFF);
    stringColor(renderer, 995, 740, "[S]ave", 0xFFFFFFFF);

    SDL_RenderPresent(renderer);
}

/* idozito fuggveny, infocrol masolva*/
Uint32 idozit(Uint32 ms, void *param) {
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;   /* ujabb varakozas */
}

//kirajzolja a tomb aktualis allapotat, majd fole a racsot
void allapot_rajzol(SDL_Window *window, SDL_Renderer *renderer, int meret_x, int** tomb, int allapot_counter) {
    //allapot kirajzolasa
    for(int i = 1; i <= meret_x; i++) {
        for(int j = 1; j <= meret_x; j++) {
            int a = (j-1)*(800/meret_x);
            int b = (i-1)*(800/meret_x);
            int c = j*(800/meret_x);
            int d = i*(800/meret_x);
            if(tomb[i][j] == 1) {
                boxColor(renderer, a, b, c, d, 0xFFFFFFFF);
            }
            else
                boxColor(renderer, a, b, c, d, 0x000000FF);
        }
    }

    //allpot counter kiirasa
    char szam[7 + 1];
    if (allapot_counter == 1000000) //egymillioig szamol, utana nullaz
        allapot_counter = 0;
    sprintf(szam, "%d", allapot_counter); //stringet tud csak kiirni, at kell adni az intet
    boxColor(renderer, 821, 21, 1078, 178, 0x000000FF);
    stringColor(renderer, 940, 100, szam, 0xFFFFFFFF);

    //grid kirajzolasa az allapot fole
    for(int i = 1; i <= meret_x; ++i) {
        lineColor(renderer, i*(800/meret_x), 0, i*(800/meret_x), 800, 0xFFFFFFFF);
    }
    for(int i = 1; i <= meret_x; ++i) {
        lineColor(renderer, 0, i*(800/meret_x), 800, i*(800/meret_x), 0xFFFFFFFF);
    }

    /* az elvegzett rajzolasok a kepernyore */
    SDL_RenderPresent(renderer);
}
