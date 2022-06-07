#ifndef LAYER_H_
#define LAYER_H_

#include <stdlib.h>
//#include <SDL.h>

typedef struct layer
{
    SDL_Surface* surface;
    int number;
    struct layer* next;
}layer;

//ajoute un nouveau calque à la fin de la liste avec la surface donné
void layer_push(struct layer* list, SDL_Surface* new_surface);

//ajoute un calque transparent à la fin de la liste avec les dimensions données
void layer_new(struct layer* list, int width, int height);

//supprime le calque du numéro donné
void layer_pop(struct layer* list, int number);

//renvoi la longueur de la liste
int layer_length(struct layer* list);

//fusionne le calque du numéro donné avec le calque précédent (c'est pour cette fonction qu il est obligatoire d avoir des calques de mêmes dimensions)
void layer_merge(struct layer* list, int number);

//pour free la liste
void layer_free(struct layer* list);

#endif

