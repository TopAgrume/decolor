#ifndef SHARED_QUEUE
#define SHARED_QUEUE

#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

typedef struct shared_queue
{
    // Used to count the number of values.
    size_t size;

    // Simple queue.
    queue* queue;
} shared_queue;

// Allocates and initializes a new shared queue.
// Returns a pointer to the new shared queue.
shared_queue* shared_queue_new();

// Pushes a new value onto the shared queue.
// queue = Address of the shared queue.
// pixel = the pixel to push (Uint32)
// 'x' and 'y', the coordinates of the pixel
void shared_queue_push(shared_queue* sq, Uint32 pixel, int x, int y);

// Pops a value off the shared queue.
// queue = Address of the shared queue.
// 'x' and 'y', the pointers coordinates of the pixel (int*)
// Returns the pixel.
Uint32 shared_queue_pop(shared_queue* sq, int* x, int* y);

// Frees the shared queue.
// queue = Address of the shared queue.
void shared_queue_destroy(shared_queue* sq);

#endif