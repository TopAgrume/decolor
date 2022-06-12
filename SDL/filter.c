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

static Uint32 moyenne(SDL_Surface *surface, int i, int j, int n)
{
    const int initial_h = SDL_max(i - n, 0);
    const int initial_w = SDL_max(j - n, 0);
    const int final_h = SDL_min(i + n, surface->h - 1);
    const int final_w = SDL_min(j + n, surface->w - 1);
    const int nb_pixel = ((final_h - initial_h) * (final_w - initial_w));

    Uint32 sum_r = 0, sum_g = 0, sum_b = 0;
    SDL_Color color;
    Uint32 pixel;

    for (i = initial_h; i < final_h; i++)
        for(j = initial_w; j < final_w; j++)
        {
            pixel = get_pixel(surface, j, i);
            SDL_GetRGB(pixel, surface->format, &color.r, &color.g, &color.b);
            sum_r += color.r;
            sum_g += color.g;
            sum_b += color.b;
        }

    return SDL_MapRGB(surface->format, sum_r / nb_pixel, sum_g / nb_pixel, sum_b / nb_pixel);
}


void blur(SDL_Surface* surface, int n){

    //Uint32 *pixels = surface->pixels;
    Uint32 pixel;

    for(int i = 0; i < surface->h; i++){
        for(int j = 0; j < surface->w; j++){
            pixel = moyenne(surface, i, j, n);
            put_pixel(surface, j, i, pixel);
        }
    }
}

void filter_color(SDL_Surface* surface, SDL_Color color){
    Uint8 r, g, b;
    Uint32 pixel;
    for(int i = 0; i < surface->w; i++){
        for(int j = 0; j < surface->h; j++){
            pixel = get_pixel(surface, i, j);
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
	    r = (color.r + r) / 2;
	    g = (color.g + g) / 2;
	    b = (color.b + b) / 2;
            pixel = SDL_MapRGB(surface->format, r, g, b);
            put_pixel(surface, i, j, pixel);
        }
    }
}


SDL_Surface* detection(SDL_Surface* surface, int n)
{
    Uint32 pixel;
    Uint32 pixel2;
    SDL_Color color;
    Uint8 r, g, b;

    //Uint32 sum_r = 0, sum_g = 0, sum_b = 0;
    SDL_Surface* copy = SDL_CreateRGBSurface(SDL_HWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);

    SDL_Rect rec;
    Uint32 bg = SDL_MapRGB(surface->format, 255, 255, 255);
    rec.x = 0;
    rec.y = 0;
    rec.w = surface->w;
    rec.h = surface->h;
    SDL_FillRect(copy, &rec, bg);

    
    for(int i = 0; i < surface->h; i++){
        for(int j = 0; j < surface->w; j++){
            pixel = moyenne(surface, i, j, n);
            SDL_GetRGB(pixel, surface->format, &color.r, &color.g, &color.b);
            pixel2 = get_pixel(surface, j, i);
            SDL_GetRGB(pixel2, surface->format, &r, &g, &b);
            pixel = SDL_MapRGB(surface->format, 255 - SDL_abs(r - color.r), 255 - SDL_abs(g - color.g), 255 - SDL_abs(b - color.b));
            //sum_r += 255 - SDL_abs(r - color.r);
            //sum_g += 255 - SDL_abs(g - color.g);
            //sum_b += 255 - SDL_abs(b - color.b);
            put_pixel(copy, j, i, pixel);
        }
    }
    
    //sum_r /= surface->w * surface->h;
    //sum_g /= surface->w * surface->h;
    //sum_b /= surface->w * surface->h;

    /* 
    for(int i = 0; i < surface->h; i++){
        for(int j = 0; j < surface->w; j++){
            pixel = get_pixel(copy, j, i);
            SDL_GetRGB(pixel,copy->format, &r, &g, &b);
            if (r < sum_r || g < sum_g || b < sum_b)
                pixel = SDL_MapRGB(copy->format, 0, 0, 0);
            else
                pixel = SDL_MapRGB(copy->format, 255, 255, 255);
            put_pixel(copy, j, i, pixel);
        }
    }*/
    
    SDL_FreeSurface(surface);
    return copy;
}


