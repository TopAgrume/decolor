#ifndef STACK
#define STACK

#include <err.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

typedef struct stack
{
    // Image
    SDL_Surface* img;

    // Pointer to the next element
    struct stack* next;
} stack;

// Stack Functions //

// Pushes a new value onto a stack.
// start = Starting address of the stack.
// img = SDL_Surface image to stack
// Returns the new starting address of the stack.
stack* stack_push(stack* start, SDL_Surface* img);

// Pops a value off at the end of a stack.
// start = Starting address of the stack.
// img = SDL_Surface image to destack
// Returns the new starting address of the stack.
stack* stack_pop_last(stack* start, SDL_Surface* img);

// Pops a value off a stack.
// start = Starting address of the stack.
// img = SDL_Surface image to destack
// Returns the new starting address of the stack.
stack* stack_pop(stack* start, SDL_Surface* img);

// Removes all the elements of a stack.
// pstart = Address that contains the starting address of the stack.
// Must set the starting address to NULL.
void stack_empty(stack** pstart);

#endif
