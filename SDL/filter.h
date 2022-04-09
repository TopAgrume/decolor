#ifndef GRAYSCALE_H_
#define GRAYSCALE_H_

#include <stdlib.h>
#include <SDL.h>

void grayscale(SDL_Surface* surface);
void negative(SDL_Surface* surface);
void color(SDL_Surface* surface, Uint8 rcons, Uint8 gcons, Uint8 bcons, Uint8 cons);

#endif
