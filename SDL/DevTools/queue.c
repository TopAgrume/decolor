#include <err.h>
#include <stdlib.h>
#include "queue.h"

// Queue Functions
queue* queue_push(queue* start, Uint32 pixel, int x, int y)
{
    struct queue* element = malloc(sizeof(struct queue));
    if (element == NULL)
        errx(EXIT_FAILURE, "Failure: malloc()");
    element->pixel = pixel;
    element->x = x;
    element->y = y;

    if (start == NULL)
        element->next = element;
    else
    {
        element->next = start->next;
        start->next = element;
    }
    return element;
}

queue* queue_pop(queue* start, Uint32* pixel, int* x, int* y)
{
    if (start == NULL)
        return NULL;
    struct queue* new = start->next;
    *pixel = new->pixel;
    *x = new->x;
    *y = new->y;
    if (new != start)
    {
        start->next = new->next;
        free(new);
        return start;
    }
    else
    {
        free(start);
        return NULL;
    }
}

void queue_empty(queue** pstart)
{
    struct queue* prev = *pstart;
    while (*pstart)
    {
        *pstart = (*pstart)->next;
        free(prev);
        prev = *pstart;
    }
}
