#include <err.h>
#include <stdlib.h>
#include "shared_queue.h"

shared_queue* shared_queue_new()
{
    struct shared_queue* newqueue = malloc(sizeof(struct shared_queue));
    if (newqueue == NULL)
        errx(EXIT_FAILURE, "Failure malloc() shared_queue");
    newqueue->size = 0;
    newqueue->queue = NULL;
    return newqueue;
}

void shared_queue_push(shared_queue* sq, Uint32 pixel, int x, int y)
{
    sq->queue = queue_push(sq->queue, pixel, x, y);
    sq->size += 1;
}

Uint32 shared_queue_pop(shared_queue* sq, int* x, int* y)
{
    Uint32 pixel;
    sq->queue = queue_pop(sq->queue, &pixel, x, y);
    sq->size -= 1;
    return pixel;
}

void shared_queue_destroy(shared_queue* sq)
{
    queue_empty(&(sq->queue));
    free(sq);
}
