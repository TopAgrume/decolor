#ifndef SHARED_STACK
#define SHARED_STACK

#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include <gtk/gtk.h>

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
// img = GdkPixbuf imgage to push
void shared_stack_push(shared_stack* ss, GdkPixbuf* img);

// Pops a value off the to of the shared stack.
// stack = Address of the shared stack.
// Returns the image.
void shared_stack_pop_last(shared_stack* ss);

// Pops a value off the end of the shared stack.
// stack = Address of the shared stack.
// Returns the image.
GdkPixbuf* shared_stack_pop(shared_stack* ss);

// Frees the shared stack.
// stack = Address of the shared stack.
void shared_stack_destroy(shared_stack* ss);

// Frees the stack but not the shared stack
void shared_stack_empty(shared_stack* ss);

#endif
