#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdbool.h>

#include "jatek.h"
#include "grafika.h"
//#include "debugmalloc.h"

int main(int argc, char *argv[]) {
    SDL_Window **window;
    SDL_Renderer **renderer;

    SDLInit(&window, &renderer);
    elrendezes(renderer);

    /* idozito hozzaadasa: 500 ms; 1000 ms / 500 ms -> 2 fps  --  infocrol masolva*/
    SDL_TimerID szim = SDL_AddTimer(500, idozit, NULL);

    /*játéktér*/
    int meret_x = 16;
    int** tomb = foglal(meret_x + 2);

    //szamlalo
    int allapot_counter = 0;

    //allapot kirajzolasa
    allapot_rajzol(window, renderer, meret_x, tomb, allapot_counter);

    /* az esemenyvezerelt hurokban hasznalt boolok */
    bool quit       = false; //kilepes
    bool sim        = false; //szimulacio
    bool paused     = true;  //szimulacio stop, allo helyzetben indul a program
    bool lep        = false; //egy lepes
    bool save       = false; //mentes
    bool load       = false; //file betoltese
    bool meret_fel  = false; //meret novelese
    bool meret_le   = false; //meret csokkentese
    bool reset      = false; //kitorli a jatekteret
    bool balklikk   = false; //egergomb figyeles
    bool jobbklikk  = false; //egergomb figyeles

    //jatekter meret vezerles
    int cnt = 0;
    int meretek[8] = {16, 20, 32, 40, 50, 80, 100, 160};

    //valtozok az eger poziciojanak figyelesehez
    int egerpoz_x = 0;
    int egerpoz_y = 0;

    //esemenyvezerelt while loop
    while (!quit) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        //vezerles
        switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym){
                    case SDLK_SPACE:
                        if (paused == false)
                            paused = true;
                        else if (paused == true)
                            paused = false;
                        break;
                    case SDLK_RIGHT:
                        lep = true;
                        break;
                    case SDLK_s:
                        save = true;
                        break;
                    case SDLK_l:
                        load = true;
                        break;
                    case SDLK_r:
                        reset = true;
                        break;
                    case SDLK_UP:
                        meret_fel = true;
                        break;
                    case SDLK_DOWN:
                        meret_le = true;
                        break;
                    break;
                }
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    balklikk = true;
                    egerpoz_x = event.button.y;
                    egerpoz_y = event.button.x;
                }
                else if (event.button.button == SDL_BUTTON_RIGHT) {
                    jobbklikk = true;
                    egerpoz_x = event.button.y;
                    egerpoz_y = event.button.x;
                }
                break;
            case SDL_USEREVENT:
                sim = true;
                break;
            /* ablak bezarasa */
            case SDL_QUIT:
                quit = true;
                break;
        }

        //jatekter editalas
        if (balklikk == true || jobbklikk == true) {
            int tabla_poz_x = ceil((egerpoz_x * meret_x) / 800);
            int tabla_poz_y = ceil((egerpoz_y * meret_x) / 800);
            if (tabla_poz_x >= 0 && tabla_poz_x <= meret_x) {
                if (tabla_poz_y >= 0 && tabla_poz_y <= meret_x) {
                    if (balklikk == true)
                        tomb[tabla_poz_x + 1][tabla_poz_y + 1] = 1;
                    else
                        tomb[tabla_poz_x + 1][tabla_poz_y + 1] = 0;
                    allapot_rajzol(window, renderer, meret_x, tomb, allapot_counter);
                }
            }
            balklikk = false;
            jobbklikk = false;
        }

        //meret inkrementalas
        if (meret_fel == true) {
            if (cnt == 7) {
                printf("Ez mar a maximalis meret.\n");
                cnt = 7;
                meret_fel = false;
            }
            else {
                cnt++;
                tomb = illeszt(meret_x, tomb, meretek[cnt]);
                meret_x = meretek[cnt];

                allapot_rajzol(window, renderer, meret_x, tomb, allapot_counter);
                meret_fel = false;
            }
        }

        //meret dekrementalas
        if (meret_le == true) {
            if (cnt <= 0) {
                cnt = 0;
                printf("Ez mar a minimalis meret.\n");
                meret_le = false;
            }
            else {
                cnt--;
                tomb = illeszt(meret_x, tomb, meretek[cnt]);
                meret_x = meretek[cnt];

                allapot_rajzol(window, renderer, meret_x, tomb, allapot_counter);
                meret_le = false;
            }
        }

        //toltes
        if (load == true) {
            felszabadit(tomb, meret_x + 2);
            tomb = input(meret_x);
            allapot_counter = 0;
            allapot_rajzol(window, renderer, meret_x, tomb, allapot_counter);
            load = false;
        }

        //mentes
        if (save == true) {
            ment(meret_x, tomb);
            save = false;
        }

        //reset
        if (reset == true) {
            felszabadit(tomb, meret_x + 2);
            tomb = empty(meret_x + 2);
            allapot_counter = 0;
            allapot_rajzol(window, renderer, meret_x, tomb, allapot_counter);
            reset = false;
        }

        //kovetkezo allapot kirajzolasa
        int** kov_allapot;
        while((sim == true && paused == false) || (paused == true && lep == true)) {
            allapot_counter++;
            kov_allapot = next_state(meret_x + 2, tomb);
            allapot_rajzol(window, renderer, meret_x, kov_allapot, allapot_counter);

            //kov_allapot tomb masolasa tomb tombbe
            for(int i = 1; i < meret_x + 1; ++i) {
                for(int j = 1; j < meret_x + 1; ++j) {
                    tomb[i][j] = kov_allapot[i][j];
                }
            }

            //kov allapot tomb felszabaditasa
            felszabadit(kov_allapot, meret_x + 2);

            sim = false;
            lep = false;
        }
    }

    //felszabaditas
    felszabadit(tomb, meret_x + 2);

    SDL_RemoveTimer(szim);

    /* ablak bezarasa */
    SDL_Quit();

    return 0;
}
