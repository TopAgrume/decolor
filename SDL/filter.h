#ifndef GRAYSCALE_H_
#define GRAYSCALE_H_

#include <stdlib.h>
//#include <SDL.h>

void grayscale(SDL_Surface* surface);
void negative(SDL_Surface* surface);
void color_filter(SDL_Surface* surface, Uint8 rcons, Uint8 gcons, Uint8 bcons, Uint8 cons);
void brightness(SDL_Surface* surface, int power);
void contrast(SDL_Surface* surface, int power);
void blur(SDL_Surface* surface, int n);
void filter_color(SDL_Surface* surface, SDL_Color color);
Uint32 moyenne(SDL_Surface *surface, int i, int j, int n);
SDL_Surface* detection(SDL_Surface* surface, int n);


#endif
