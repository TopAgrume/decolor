#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "layer.h"

void layer_push(layer* list, SDL_Surface* new_surface){
	struct layer* loop = list;
	struct layer* new = malloc(sizeof(struct layer));
	if (new == NULL)
		errx(EXIT_FAILURE, "Failure: malloc() layer_push");
	new->surface = new_surface;
	list->number++;
	new->number = list->number;
	new->next = NULL;

	while(loop->next)
		loop = loop->next;
	loop->next = new;
}

//void layer_new(layer* list, int width, int height){
	//new_surface = une nouvelle surface transparente;
	//layer_push(list, new_surface);
	//TODO
//}

void layer_pop(layer* list, int number){
	struct layer* loop = list;
	while(loop->next && loop->next->number != number)
		loop = loop->next;

	if(loop->next->number == number){
		struct layer* tofree = loop->next;
		loop->next = loop->next->next;
		free(tofree);
	}
}

void layer_merge(layer* list, int number){
	struct layer* loop = list;
        while(loop->next && loop->next->number != number)
                loop = loop->next;

	if(loop->next->number == number){
		SDL_BlitSurface(loop->next->surface, NULL, loop->surface, NULL);
		layer_pop(list, number);
	}
}

int layer_length(layer* list){
	int nb = 0;
	struct layer* loop = list->next;
	while(loop){
		loop = loop->next;
		nb++;
	}
	return nb;
}

void layer_free(layer* list){
	struct layer* loop;
	while(list->next){
		loop = list->next;
		list->next = list->next->next;
		free(loop);
	}
	free(list);
}
