#ifndef SHAPE_H
#define SHAPE_H

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "pixel_operations.h"
#include "tools.h"
#include <math.h>

SDL_Surface* make_fill_square(SDL_Surface* img, int x1, int y1, int x2, int y2, SDL_Color new_color);
void make_horizontal(SDL_Surface* img, int x1, int x2, int y, SDL_Color new_color);
void make_vetical(SDL_Surface* img, int y1, int y2, int x, SDL_Color new_color);
SDL_Surface* make_empty_square(SDL_Surface* img, int x1, int y1, int x2, int y2, SDL_Color new_color, int size);
SDL_Surface* make_empty_triangle(SDL_Surface* img, int x1, int y1, int x2, int y2, SDL_Color new_color, int size);
SDL_Surface* bresenham_circle(SDL_Surface* img, int x1, int y1, int x2, int y2, SDL_Color color, int size);

#endif
