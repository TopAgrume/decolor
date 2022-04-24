#include <err.h>
#include <stdlib.h>
#include "shared_stack.h"


SDL_Surface* copy_image(SDL_Surface* img)
{
    Uint32 pixel;
    SDL_Surface *copy;
    copy = SDL_CreateRGBSurface(SDL_HWSURFACE, img->w, img->h, img->format->BitsPerPixel, img->format->Rmask, img->format->Gmask, img->format->Bmask, img->format->Amask);

    if(copy == NULL || img == NULL)
        errx(EXIT_FAILURE, "Copy_Image() NULL\n");
    int x, y;
    for (x = 0; x < img->w; x++)
    {
        for (y = 0; y < img->h; y++)
        {
            pixel = get_pixel(img, x, y);
            put_pixel(copy, x, y, pixel);
        }
    }

    return copy;
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
    // Save 15 moves max
    if (ss->size >= 15)
        ss->stack = stack_pop_last(ss->stack);
    else
        ss->size += 1;
    ss->stack = stack_push(ss->stack, copy_image(img));
}

void shared_stack_pop_last(shared_stack* ss)
{
    if (ss->size == 0)
        errx(EXIT_FAILURE, "Stack already empty\n");
    ss->stack = stack_pop_last(ss->stack);
    //ss->size -= 1;
}

SDL_Surface* shared_stack_pop(shared_stack* ss)
{
    if (ss->size == 0)
        errx(EXIT_FAILURE, "Stack already empty\n");
    SDL_Surface* img = copy_image(ss->stack->img);
    ss->stack = stack_pop(ss->stack);
    ss->size -= 1;
    return img;
}

void shared_stack_destroy(shared_stack* ss)
{
    stack_empty(&(ss->stack));
    free(ss);
}

void shared_stack_empty(shared_stack* ss)
{
    stack_empty(&(ss->stack));
    ss->size = 0;
}
