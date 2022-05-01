#ifndef GTK_H_
#define GTK_H_

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <glib.h>

int create_window_decolor(int argc, char* argv[]);

/*typedef struct shared_stack
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
// img = GtkWidget imgage to push
void shared_stack_push(shared_stack* ss, GtkWidget* img);

// Pops a value off the to of the shared stack.
// stack = Address of the shared stack.
// Returns the image.
void shared_stack_pop_last(shared_stack* ss);

// Pops a value off the end of the shared stack.
// stack = Address of the shared stack.
// Returns the image.
GtkWidget* shared_stack_pop(shared_stack* ss);

// Frees the shared stack.
// stack = Address of the shared stack.
void shared_stack_destroy(shared_stack* ss);

// Frees the stack but not the shared stack
void shared_stack_empty(shared_stack* ss);
*/

#endif
