#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "pixel_operations.h"
#include "filter.h"
#include "tools.h"
#include "DevTools/shared_stack.h" 

//#define TEST_FILTER
//#define TEST_SEAL_PIPETTE
#define TEST_STACK

int main(int argc , char* argv[])
{
    if(argc != 2)
	    errx(EXIT_FAILURE, "Il faut le chemin de l'image");

    char* path = argv[1];
    
#ifdef TEST_FILTER
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image(path);
    screen_surface = display_image(image_surface);

    wait_for_keypressed();

    grayscale(image_surface);

    screen_surface = display_image(image_surface);

    wait_for_keypressed();
    
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
#endif

#ifdef TEST_SEAL_PIPETTE
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image(path);
    screen_surface = display_image(image_surface);

    wait_for_keypressed();
    SDL_Color color;
    color.r = 0;
    color.g = 145;
    color.b = 255;
    filling_seal(image_surface, 0, 0, color, 200);
    screen_surface = display_image(image_surface);

    wait_for_keypressed();

    filling_seal(image_surface, 500, 500, color, 200);
    color = pipette(image_surface, 500, 500);
    printf("pipette R: %i, G: %i, B: %i\n", color.r, color.g, color.b);
    screen_surface = display_image(image_surface);
    wait_for_keypressed();
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
#endif
#ifdef TEST_STACK
    // Initialize SDL + screen
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;
    init_sdl();
    //SDL_Surface img;
    image_surface = load_image(path);
    
    // Initialize stacks
    shared_stack* stack_back = shared_stack_new();
    shared_stack* stack_after = shared_stack_new();
    printf("Queue/Stacks Initialize\n");
    
    // Initilize Colors
    SDL_Color color1;
    color1.r = 0; 
    color1.g = 255;
    color1.b = 255;
    
    SDL_Color color2;
    color2.r = 255; 
    color2.g = 0;
    color2.b = 255;
    
    SDL_Color color3;
    color3.r = 255; 
    color3.g = 255;
    color3.b = 0;

    SDL_Color color4;
    color4.r = 0; 
    color4.g = 0;
    color4.b = 255;
    
    SDL_Color color5;
    color5.r = 0; 
    color5.g = 255;
    color5.b = 0;
    
    SDL_Color color6;
    color6.r = 255; 
    color6.g = 0;
    color6.b = 0;
    
    // Stack images in back_stack: 1st element (start)
    shared_stack_push(stack_back, image_surface);
    screen_surface = display_image(image_surface);
    printf("Number in stack_before: %li\n", stack_back->size);
    printf("Number in stack_after: %li\n", stack_after->size);
    wait_for_keypressed();

    // 2nd element
    image_surface = filling_seal(image_surface, 0, 0, color1, 200);
    shared_stack_push(stack_back, image_surface);
    update_surface(screen_surface, image_surface);
    printf("Number in stack_before: %li\n", stack_back->size);
    printf("Number in stack_after: %li\n", stack_after->size);
    wait_for_keypressed();

    
    // 3rd element
    image_surface = filling_seal(image_surface, 0, 0, color2, 200);
    shared_stack_push(stack_back, image_surface);
    update_surface(screen_surface, image_surface);
    printf("Number in stack_before: %li\n", stack_back->size);
    printf("Number in stack_after: %li\n", stack_after->size);
    wait_for_keypressed();
    
    // 4th element
    image_surface = filling_seal(image_surface, 500, 500, color3, 200);
    shared_stack_push(stack_back, image_surface);
    update_surface(screen_surface, image_surface);
    printf("Number in stack_before: %li\n", stack_back->size);
    printf("Number in stack_after: %li\n", stack_after->size);
    wait_for_keypressed();
    
    // 5th element
    image_surface = filling_seal(image_surface, 500, 500, color4, 200);
    shared_stack_push(stack_back, image_surface);
    update_surface(screen_surface, image_surface);
    printf("Number in stack_before: %li\n", stack_back->size);
    printf("Number in stack_after: %li\n", stack_after->size);
    wait_for_keypressed();
    
    // 6th element
    image_surface = filling_seal(image_surface, 0, 0, color5, 200);
    shared_stack_push(stack_back, image_surface);
    update_surface(screen_surface, image_surface);
    printf("Number in stack_before: %li\n", stack_back->size);
    printf("Number in stack_after: %li\n", stack_after->size);
    wait_for_keypressed();

    // 7th element
    image_surface = filling_seal(image_surface, 500, 500, color6, 200);
    shared_stack_push(stack_back, image_surface);
    update_surface(screen_surface, image_surface);
    printf("Number in stack_before: %li\n", stack_back->size);
    printf("Number in stack_after: %li\n", stack_after->size);
    wait_for_keypressed();

    shared_stack_empty(stack_back);
    SDL_Surface recup;
    SDL_Surface* try = NULL;
    // Destack images in after_stack: 1st element (start)
    while (stack_back->size > 1)
    {
        recup = shared_stack_pop(stack_back);
        try = &recup;
        update_surface(screen_surface, try);
        shared_stack_push(stack_after, try);
        printf("Number in stack_before: %li\n", stack_back->size);
        printf("Number in stack_after: %li\n", stack_after->size);
        wait_for_keypressed();
    }
    
    printf("Number in stack_before: %li\n", stack_back->size);
    printf("Number in stack_after: %li\n", stack_after->size);
    shared_stack_destroy(stack_back);
    shared_stack_destroy(stack_after);
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
    //SDL_FreeSurface(try);


#endif
    return 0;
}

