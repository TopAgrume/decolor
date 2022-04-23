#ifndef STACK
#define STACK

#include <err.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "../pixel_operations.h"
#include <gtk/gtk.h>

typedef struct stack
{
    // Image
    GtkWidget* img;

    // Pointer to the next element
    struct stack* next;
} stack;

// Stack Functions //

// Pushes a new value onto a stack.
// start = Starting address of the stack.
// img = GtkWidget image to stack
// Returns the new starting address of the stack.
stack* stack_push(stack* start, GtkWidget* img);

// Pops a value off at the end of a stack.
// start = Starting address of the stack.
// img = GtkWidget image to destack
// Returns the new starting address of the stack.
stack* stack_pop_last(stack* start);

// Pops a value off a stack.
// start = Starting address of the stack.
// img = GtkWidget image to destack
// Returns the new starting address of the stack.
stack* stack_pop(stack* start);

// Removes all the elements of a stack.
// pstart = Address that contains the starting address of the stack.
// Must set the starting address to NULL.
void stack_empty(stack** pstart);

#endif
