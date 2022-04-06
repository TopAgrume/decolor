#ifndef Tools_H
#define Tools_H

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "DevTools/queue.h"
#include "DevTools/shared_queue.h"
#include "pixel_operations.h"

SDL_Surface* filling_seal(SDL_Surface* img, int x, int y, SDL_Color new_color);
SDL_Color pipette(SDL_Surface* img, int x, int y);

#endif
