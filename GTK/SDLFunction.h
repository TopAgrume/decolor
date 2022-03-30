#ifndef SDL_Function_H
#define SDL_Function_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>

void init_sdl();
SDL_Surface* displayImage(SDL_Surface* img);
SDL_Surface* loadImage(char* path);
SDL_Surface* copy_image(SDL_Surface* img);

// Filtres
void RGB2BLACK(SDL_Surface* image)
void RGB2GRAY(SDL_Surface* image)


#endif