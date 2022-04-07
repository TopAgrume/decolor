#include <err.h>
#include <stdlib.h>
#include "stack.h"

// Stack Functions
stack* stack_push(stack* start, SDL_Surface* img)
{
    struct stack* element = malloc(sizeof(struct stack));
    if (element == NULL)
        errx(EXIT_FAILURE, "Failure: malloc() stack_push");
    element->img = img;

    if (start == NULL)
        element->next = element;
    else
        element->next = start;
    return element;
}

stack* stack_pop_last(stack* start, SDL_Surface* img)
{
    if (start == NULL)
        return NULL;
    if (img->h < 0)
        return NULL;

    stack* p = NULL;
    stack* before_p = NULL;
    for (p = start; p->next != NULL;)
    {
        before_p = p;
        p = p->next;
    }
    img = p->img;
    before_p->next = NULL;
    free(p);
    return start;
}

stack* stack_pop(stack* start, SDL_Surface* img)
{
    if (start == NULL)
        return NULL;
    if (img->h < 0)
        return NULL;
    struct stack* new = start;
    stack* p = new->next;
    img = new->img;
    free(new);
    return p;
}

void stack_empty(stack** pstart)
{
    struct stack* prev = *pstart;
    while (*pstart)
    {
        *pstart = (*pstart)->next;
        free(prev);
        prev = *pstart;
    }
}
