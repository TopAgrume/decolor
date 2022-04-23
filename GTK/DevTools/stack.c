#include <err.h>
#include <stdlib.h>
#include "stack.h"

// Stack Functions
stack* stack_push(stack* start, GdkPixbuf* img)
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
    if (before_p != NULL)
    {
        before_p->next = NULL;
        gdk_pixbuf_unref(p->img);
        free(p);
        return start;
    }
    else
    {
        gdk_pixbuf_unref(p->img);
        free(p);
        return NULL;
    }
}

stack* stack_pop(stack* start)
{
    if (start == NULL)
        errx(EXIT_FAILURE, "Nothing to pop from the stack");
    struct stack* new = start;
    start = start->next;
    gdk_pixbuf_unref(new->img);
    free(new);
    return start;
}

void stack_empty(stack** pstart)
{
    struct stack* prev = *pstart;
    while (*pstart)
    {
        *pstart = (*pstart)->next;
        gdk_pixbuf_unref(prev->img);
        free(prev);
        prev = *pstart;
    }
}
