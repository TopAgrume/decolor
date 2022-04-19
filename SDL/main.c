#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "pixel_operations.h"
#include "filter.h"
#include "tools.h"
#include "DevTools/shared_stack.h"
#include "shape.h"

//#define TEST_FILTER
//#define TEST_SEAL_PIPETTE
//#define TEST_STACK
//#define TEST_BRUSH
//#define TEST_CROP
//#define TEST_TRANSFORM
//#define TEST_SHAPE
//#define TEST_TRIANGLE
#define TEST_CIRCLE

int main(int argc , char* argv[])
{
    if(argc != 2)
	    errx(EXIT_FAILURE, "Il faut le chemin de l'image");

    char* path = argv[1];

#ifdef TEST_CIRCLE
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image(path);
    screen_surface = display_image(image_surface);
    wait_for_keypressed();
    
    SDL_Color color;
    color.r = 145;
    color.g = 145;
    color.b = 0;
    SDL_Color color1;
    color1.r = 145;
    color1.g = 0;
    color1.b = 145;
    SDL_Color color2;
    color2.r = 0;
    color2.g = 145;
    color2.b = 145;
    SDL_Color color3;
    color3.r = 255;
    color3.g = 0;
    color3.b = 0;
    
    bresenham_circle(image_surface, 200, 200, 300, 300, color, 5);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();
    
    bresenham_circle(image_surface, 600, 600, 300, 300, color1, 10);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    bresenham_circle(image_surface, 300, 600, 600, 300, color2, 10);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    bresenham_circle(image_surface, 500, 200, 200, 600, color3, 10);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();
    
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

#endif

#ifdef TEST_FILTER
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image(path);
    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    negative(image_surface);
    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
#endif

#ifdef TEST_CROP
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image(path);
    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    SDL_Surface* image_surface2 = crop(image_surface, 200, 200, 400, 200);
    screen_surface = display_image(image_surface2);
    wait_for_keypressed();

    SDL_FreeSurface(image_surface2);
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
#endif

#ifdef TEST_TRANSFORM
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image(path);
    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    SDL_Surface* image_surface2 = rotozoomSurface(image_surface, 0, 0.5, 0);
    screen_surface = display_image(image_surface2);
    wait_for_keypressed();

    SDL_FreeSurface(image_surface2);
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
#endif

#ifdef TEST_TRIANGLE
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image(path);
    screen_surface = display_image(image_surface);
    wait_for_keypressed();
   
    SDL_Color color;
    color.r = 145;
    color.g = 145;
    color.b = 0;
    SDL_Color color1;
    color1.r = 145;
    color1.g = 0;
    color1.b = 145;
    SDL_Color color2;
    color2.r = 0;
    color2.g = 145;
    color2.b = 145;
    SDL_Color color3;
    color3.r = 255;
    color3.g = 0;
    color3.b = 0;

    make_empty_triangle(image_surface, 30, 30, 700, 700, color, 10);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();
    
    make_empty_triangle(image_surface, 600, 50, 50, 600, color1, 10);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    make_empty_triangle(image_surface, 200, 700, 1200, 30, color2, 10);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    make_empty_triangle(image_surface, 1100, 700, 200, 10, color3, 10);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();
    
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
#endif

#ifdef TEST_SHAPE
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image(path);
    screen_surface = display_image(image_surface);
    wait_for_keypressed();
    
    SDL_Color color;
    color.r = 145;
    color.g = 145;
    color.b = 0;
    SDL_Color color1;
    color1.r = 145;
    color1.g = 0;
    color1.b = 145;
    SDL_Color color2;
    color2.r = 0;
    color2.g = 145;
    color2.b = 145;
    SDL_Color color3;
    color3.r = 255;
    color3.g = 0;
    color3.b = 0;


    make_empty_square(image_surface, 10, 10, 1200, 700, color, 10);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    make_empty_square(image_surface, 1200, 10, 10, 700, color1, 10);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    make_empty_square(image_surface, 10, 700, 1200, 10, color2, 10);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    make_empty_square(image_surface, 1200, 700, 10, 10, color3, 10);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    make_fill_square(image_surface, 10, 10, 1204, 706, color);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    make_fill_square(image_surface, 1180, 30, 30, 690, color1);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    make_fill_square(image_surface, 50, 650, 1150, 50, color2);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    make_fill_square(image_surface, 1120, 620, 70, 70, color3);
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

#endif

#ifdef TEST_BRUSH
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;
    int width = 600;
    int height = 600;

    init_sdl();

    printf("%s\n",path);
    image_surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 255);
    SDL_FillRect(image_surface, NULL, SDL_MapRGB(image_surface->format, 0, 0, 0));
    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    SDL_Color color;
    color.r = 255;
    color.g = 255;
    color.b = 255;

    drawline(image_surface, color, 20, 20, 500, 500, 2);
    drawline(image_surface, color, 20, 500, 500, 500, 2);
    drawline(image_surface, color, 500, 20, 20, 500, 2);
    drawline(image_surface, color, 500, 20, 20, 20, 2);

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
    screen_surface = display_image(image_surface);
    shared_stack_push(stack_back, image_surface);
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
    SDL_FreeSurface(image_surface);

    // Destack images in after_stack: 1st element (start)
    while (stack_back->size > 0)
    {
        image_surface = shared_stack_pop(stack_back);
        update_surface(screen_surface, image_surface);
        shared_stack_push(stack_after, image_surface);
        printf("Number in stack_before: %li\n", stack_back->size);
        printf("Number in stack_after: %li\n", stack_after->size);
        wait_for_keypressed();
        SDL_FreeSurface(image_surface);
    }
    shared_stack_empty(stack_after);

    printf("Number in stack_before: %li\n", stack_back->size);
    printf("Number in stack_after: %li\n", stack_after->size);

    shared_stack_destroy(stack_back);
    shared_stack_destroy(stack_after);
    //SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

#endif
    return 0;
}

