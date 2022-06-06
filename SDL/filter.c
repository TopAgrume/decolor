#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "pixel_operations.h"

//the grayscale filter
// -> 'surface' the image to apply the filter
void grayscale(SDL_Surface* surface){
    Uint8 r, g, b, gris;
    Uint32 pixel;
    for(int i = 0; i < surface->w; i++){
            for(int j = 0; j < surface->h; j++){
                    pixel = get_pixel(surface, i, j);
                    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                    gris = 0.3*r + 0.59*g + 0.11*b;
                    pixel = SDL_MapRGB(surface->format, gris, gris, gris);
                    put_pixel(surface, i, j, pixel);
            }
    }
}

//the negative filter
// -> 'surface' the image to apply the filter
void negative(SDL_Surface* surface){
    Uint8 r, g, b;
    Uint32 pixel;
    for(int i = 0; i < surface->w; i++){
            for(int j = 0; j < surface->h; j++){
                    pixel = get_pixel(surface, i, j);
                    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                    r = 255 - r;
		    g = 255 - g;
		    b = 255 - b;
                    pixel = SDL_MapRGB(surface->format, r, g, b);
                    put_pixel(surface, i, j, pixel);
            }
    }
}

void brightness(SDL_Surface* surface, int power){
    Uint8 f(Uint8 c, int n){
	    if(c + n > 255)
	    	    return 255;
	    else if(c + n < 0)
		    return 0;
	    else
		    return c + n;
    }

    Uint8 r, g, b;
    Uint32 pixel;
    for(int i = 0; i < surface->w; i++){
            for(int j = 0; j < surface->h; j++){
                    pixel = get_pixel(surface, i, j);
                    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                    r = f(r, power);
                    g = f(g, power);
                    b = f(b, power);
                    pixel = SDL_MapRGB(surface->format, r, g, b);
                    put_pixel(surface, i, j, pixel);
            }
    }
}

void contrast(SDL_Surface* surface, int power){
    double test_pow(double x, int n){
            double res = 1;
            for(int i = 0; i < n; i++)
                    res *= x;
            return res;
    }

    Uint8 f(Uint8 c, int n){
            if(c <= 255 / 2)
		    return (Uint8)( (255/2) * test_pow((double) 2 * c / 255, n));
	    else
		    return 255 - f(255 - c, n);
    }

    Uint8 r, g, b;
    Uint32 pixel;
    for(int i = 0; i < surface->w; i++){
            for(int j = 0; j < surface->h; j++){
                    pixel = get_pixel(surface, i, j);
                    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                    r = f(r, power);
                    g = f(g, power);
                    b = f(b, power);
                    pixel = SDL_MapRGB(surface->format, r, g, b);
                    put_pixel(surface, i, j, pixel);
            }
    }
}

//A customizable filter
// -> 'surface' the img to apply the filter
// -> 'rcons', 'gcons' and 'bcons' booleans to know which colors are constants
// -> 'cons' the minimum threshold for the constants colors
void color_filter(SDL_Surface* surface, Uint8 rcons, Uint8 gcons, Uint8 bcons, Uint8 cons){
    if((rcons != 0 && rcons != 1) || (gcons != 0 && gcons != 1) || (bcons != 0 && bcons != 1))
	    errx(EXIT_FAILURE, "color() : rcons/gcons/bcons is a boolean !");
    Uint8 r, g, b;
    Uint32 pixel;
    for(int i = 0; i < surface->w; i++){
            for(int j = 0; j < surface->h; j++){
                    pixel = get_pixel(surface, i, j);
                    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                    pixel = SDL_MapRGB(surface->format, r * rcons + (1-rcons) * cons, g * gcons + (1-gcons) * cons, b * bcons + (1-bcons) * cons);
                    put_pixel(surface, i, j, pixel);
            }
    }
}

