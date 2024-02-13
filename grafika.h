#ifndef GRAFIKA_H_INCLUDED
#define GRAFIKA_H_INCLUDED

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

void SDLInit(SDL_Window **pwindow, SDL_Renderer **prenderer);
void elrendezes(SDL_Renderer *renderer);
Uint32 idozit(Uint32 ms, void *param);
void allapot_rajzol(SDL_Window *window, SDL_Renderer *renderer, int meret_x, int** tomb, int allapot_counter);

#endif // GRAFIKA_H_INCLUDED
