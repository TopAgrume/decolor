#ifndef QUEUE
#define QUEUE

typedef struct queue
{
    // Pixel
    Uint32 pixel;

    // Coordinates of pixel
    int x;
    int y;
    // Pointer to the next element.
    struct queue* next;
} queue;

// Queue Functions //

// Pushes a new value onto a queue.
// start = Starting address of the queue.
// x, y = Coordinates to push.
// Returns the new starting address of the queue.
queue* queue_push(queue* start, Uint32 pixel, int x, int y);

// Pops a value off a queue.
// start = Starting address of the queue.
// x, y = Pointer used to return the coordinates.
// Returns the new starting address of the queue.
queue* queue_pop(queue* start, Uint32* pixel, int* x, int* y);

// Removes all the elements of a queue.
// pstart = Address that contains the starting address of the queue.
// Must set the starting address to NULL.
void queue_empty(queue** pstart);

#endif
