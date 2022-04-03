#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "pixel_operations.h"
#include "filter.h"

int main(int argc , char* argv[])
{
    if(argc != 2)
	    errx(EXIT_FAILURE, "Il faut le chemin de l'image");

    char* path = argv[1];
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

}

