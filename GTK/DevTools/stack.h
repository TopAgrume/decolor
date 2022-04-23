#ifndef STACK
#define STACK

#include <err.h>
#include <stdlib.h>
#include <gtk/gtk.h>

typedef struct stack
{
    // Image
    GdkPixbuf* img;

    // Pointer to the next element
    struct stack* next;
} stack;

// Stack Functions //

// Pushes a new value onto a stack.
// start = Starting address of the stack.
// img = GdkPixbuf image to stack
// Returns the new starting address of the stack.
stack* stack_push(stack* start, GdkPixbuf* img);

// Pops a value off at the end of a stack.
// start = Starting address of the stack.
// img = GdkPixbuf image to destack
// Returns the new starting address of the stack.
stack* stack_pop_last(stack* start);

// Pops a value off a stack.
// start = Starting address of the stack.
// img = GdkPixbuf image to destack
// Returns the new starting address of the stack.
stack* stack_pop(stack* start);

// Removes all the elements of a stack.
// pstart = Address that contains the starting address of the stack.
// Must set the starting address to NULL.
void stack_empty(stack** pstart);

#endif
