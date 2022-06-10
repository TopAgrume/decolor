#ifndef Tools_H
#define Tools_H

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL/SDL_rotozoom.h>
#include "DevTools/queue.h"
#include "DevTools/shared_queue.h"
#include "pixel_operations.h"

SDL_Surface* filling_seal(SDL_Surface* img, int x, int y, SDL_Color new_color, int threshold);
SDL_Color pipette(SDL_Surface* img, int x, int y);
void point(SDL_Surface* surface, SDL_Color color, int x, int y, int size);
void drawline(SDL_Surface* img, SDL_Color color, int x1, int y1, int x2, int y2, int size);
//void brush(SDL_Surface* surface, SDL_Color color, int x, int y);
SDL_Surface* crop(SDL_Surface* surface, int x, int y, int width, int height);
SDL_Surface* reversion(SDL_Surface* surface, int horizontal, int vertical);

//Autres fonctions
SDL_Surface* rotate(SDL_Surface* surface, int sens);
void point_image(SDL_Surface* surface, SDL_Surface* img,  int x, int y, int size);
void drawline_image(SDL_Surface* img, SDL_Surface* img2, int x1, int y1, int x2, int y2, int size);
SDL_Surface* resize_image(SDL_Surface* surface, int x, int y);

// Copy Paste
void past_selection(SDL_Surface* img, SDL_Surface* surface, int x, int y);
SDL_Surface* copy_selection(SDL_Surface* surface, int x, int y, int width, int height);
void erase_selection(SDL_Surface* surface, int x, int y, int width, int height);

#endif
