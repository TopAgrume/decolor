#include <err.h>
#include <stdlib.h>
#include "shared_stack.h"

shared_stack* shared_stack_new()
{
    struct shared_stack* newstack = malloc(sizeof(struct shared_stack));
    if (newstack == NULL)
        errx(EXIT_FAILURE, "Failure malloc() shared_stack");
    newstack->size = 0;
    newstack->stack = NULL;
    return newstack;
}

void shared_stack_push(shared_stack* ss, SDL_Surface img)
{
    ss->stack = stack_push(sq->stack, img);
    ss->size += 1;
}

SDL_Surface shared_stack_pop_last(shared_stack* ss)
{
    SDL_Surface img;
    ss->stack = stack_pop_last(sq->stack, &img);
    ss->size -= 1;
    return img;
}

SDL_Surface shared_stack_pop(shared_stack* ss, SDL_Surface* img)
{
    SDL_Surface img;
    ss->stack = stack_pop(sq->stack, &img);
    ss->size -= 1;
    return img;
}

void shared_stack_destroy(shared_stack* ss)
{
    stack_empty(&(ss->stack));
    free(sq);
}