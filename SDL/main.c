#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "pixel_operations.h"
#include "filter.h"
#include "tools.h"

//#define TEST_FILTER
#define TEST_SEAL_PIPETTE


int main(int argc , char* argv[])
{
    if(argc != 2)
	    errx(EXIT_FAILURE, "Il faut le chemin de l'image");

    char* path = argv[1];
    
#ifdef TEST_FILTER
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image(path);
    screen_surface = display_image(image_surface);

    wait_for_keypressed();

    grayscale(image_surface);

    screen_surface = display_image(image_surface);

    wait_for_keypressed();
    
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
#endif

#ifdef TEST_SEAL_PIPETTE
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image(path);
    screen_surface = display_image(image_surface);

    wait_for_keypressed();
    SDL_Color color;
    color.r = 0;
    color.g = 145;
    color.b = 255;
    filling_seal(image_surface, 0, 0, color, 200);
    screen_surface = display_image(image_surface);

    wait_for_keypressed();

    filling_seal(image_surface, 500, 500, color, 200);
    color = pipette(image_surface, 500, 500);
    printf("pipette R: %i, G: %i, B: %i\n", color.r, color.g, color.b);
    screen_surface = display_image(image_surface);
    wait_for_keypressed();
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
#endif
    return 0;
}

