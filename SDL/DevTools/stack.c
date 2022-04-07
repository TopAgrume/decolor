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
        element->next = NULL;
    else
        element->next = start;
    return element;
}

stack* stack_pop_last(stack* start)
{
    if (start == NULL)
        errx(EXIT_FAILURE, "Nothing to pop from the stack");

    stack* p = NULL;
    stack* before_p = NULL;
    for (p = start; p->next != NULL;)
    {
        before_p = p;
        p = p->next;
    }
    before_p->next = NULL;
    free(p);
    return start;
}

stack* stack_pop(stack* start, SDL_Surface* img)
{
    if (start == NULL)
        errx(EXIT_FAILURE, "Nothing to pop from the stack");
    *img = *(start->img);
    struct stack* new = start;
    start = start->next;

    free(new);
    return start;
}

void stack_empty(stack** pstart)
{
    struct stack* prev = *pstart;
    while (*pstart)
    {
        *pstart = (*pstart)->next;
        SDL_FreeSurface(prev->img);
        free(prev);
        prev = *pstart;
    }
}
