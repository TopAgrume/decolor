#include <err.h>
#include <stdlib.h>
#include "shared_stack.h"


SDL_Surface* copy_image(SDL_Surface* img)
{
    Uint32 pixel;
    SDL_Surface* copy;
    copy = SDL_CreateRGBSurface(SDL_HWSURFACE,
        img->w,
        img->h,
        img->format->BitsPerPixel, 0, 0, 0, 0);
    for (int i = 0; i < img->w; i++)
    {
        for (int j = 0; j < img->h; j++)
        {
            pixel = get_pixel(img, i, j);
            put_pixel(copy, i, j, pixel);
        }
    }
    return(copy);
}

shared_stack* shared_stack_new()
{
    struct shared_stack* newstack = malloc(sizeof(struct shared_stack));
    if (newstack == NULL)
        errx(EXIT_FAILURE, "Failure malloc() shared_stack");
    newstack->size = 0;
    newstack->stack = NULL;
    return newstack;
}

void shared_stack_push(shared_stack* ss, SDL_Surface* img)
{
    ss->stack = stack_push(ss->stack, copy_image(img));
    ss->size += 1;
}

SDL_Surface shared_stack_pop_last(shared_stack* ss)
{
    SDL_Surface img;
    ss->stack = stack_pop_last(ss->stack, &img);
    ss->size -= 1;
    return img;
}

SDL_Surface* shared_stack_pop(shared_stack* ss)
{
    SDL_Surface* img;
    ss->stack = stack_pop(ss->stack, img);
    ss->size -= 1;
    return img;
}

void shared_stack_destroy(shared_stack* ss)
{
    stack_empty(&(ss->stack));
    free(ss);
}