#ifndef GRAYSCALE_H_
#define GRAYSCALE_H_

#include <stdlib.h>
//#include <SDL.h>

void grayscale(SDL_Surface* surface);
void negative(SDL_Surface* surface);
void color_filter(SDL_Surface* surface, Uint8 rcons, Uint8 gcons, Uint8 bcons, Uint8 cons);
void brightness(SDL_Surface* surface, int power);
void contrast(SDL_Surface* surface, int power);

#endif
