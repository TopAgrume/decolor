#ifndef SHARED_STACK
#define SHARED_STACK

#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "../pixel_operations.h"

typedef struct shared_stack
{
    // Used to count the number of values.
    size_t size;

    // Simple stack.
    stack* stack;
} shared_stack;

// Allocates and initializes a new shared stack.
// Returns a pointer to the new shared stack.
shared_stack* shared_stack_new();

// Pushes a new value onto the shared stack.
// stack = Address of the shared stack.
// img = SDL_Surface imgage to push
void shared_stack_push(shared_stack* ss, SDL_Surface* img);

// Pops a value off the to of the shared stack.
// stack = Address of the shared stack.
// Returns the image.
SDL_Surface shared_stack_pop_last(shared_stack* ss);

// Pops a value off the end of the shared stack.
// stack = Address of the shared stack.
// Returns the image.
void shared_stack_pop(shared_stack* ss, SDL_Surface* img);

// Frees the shared stack.
// stack = Address of the shared stack.
void shared_stack_destroy(shared_stack* ss);

#endif
