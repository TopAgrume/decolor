#include "tools.h"

// This function returns the image after using the seal (Format SDL_Surface*)
// -> 'img' the pointer on SDL_Surface (image)
// -> 'x' and 'y' coordinates of the chosen pixel
SDL_Surface* filling_seal(SDL_Surface* img, int x, int y, SDL_Color new_color)
{
    // Initialization
    SDL_Color color;
    color.r = 0;
    color.g = 0;
    color.b = 0;
    printf("Initialization seal\n");

    // Try the value of the first pixel
    Uint32 pixel = get_pixel(img, x, y);
    SDL_GetRGB(pixel, img->format, &(color.r), &(color.g), &(color.b));
    if (color.r == new_color.r && color.g == new_color.g && color.b == new_color.b)
        return img;
    
    // If colors are different, then apply the seal tool
    shared_queue* seal_queue = shared_queue_new();
    shared_queue_push(seal_queue, pixel, x, y);
    
    int dequeue_x = 0;
    int dequeue_y = 0;
    int neighbour_x = 0;
    int neighbour_y = 0;
    while (seal_queue->size != 0)
    {
        pixel = shared_queue_pop(seal_queue, &dequeue_x, &dequeue_y);
        SDL_GetRGB(pixel, img->format, &(color.r), &(color.g), &(color.b));
        pixel = SDL_MapRGB(img->format, new_color.r, new_color.g, new_color.b);
        put_pixel(img, dequeue_x, dequeue_y, pixel);
        printf("Diffuse x = %i, y = %i;\n", dequeue_x, dequeue_y);

        for (int move = -1; move < 2; move += 2)
        {
            neighbour_x = dequeue_x + move;
            neighbour_y = dequeue_y + move;
            if (0 <= neighbour_x && neighbour_x < img->w)
            {
                pixel = get_pixel(img, neighbour_x, dequeue_y);
                SDL_GetRGB(pixel, img->format, &(color.r), &(color.g), &(color.b));
                if (color.r != new_color.r || color.g != new_color.g || color.b != new_color.b)
                    shared_queue_push(seal_queue, pixel, neighbour_x, dequeue_y);
            }
            if (0 <= neighbour_y && neighbour_y < img->h)
            {
                pixel = get_pixel(img, dequeue_x, neighbour_y);
                SDL_GetRGB(pixel, img->format, &(color.r), &(color.g), &(color.b));
                if (color.r != new_color.r || color.g != new_color.g || color.b != new_color.b)
                    shared_queue_push(seal_queue, pixel, dequeue_x, neighbour_y);
            }
        }
    }

    return img;
}

// This function returns the color of the pixel selected (Format SDL_Color)
// -> 'img' the pointer on SDL_Surface (image)
// -> 'x' and 'y' coordinates of the chosen pixel
SDL_Color pipette(SDL_Surface* img, int x, int y)
{
    // Initialization
    SDL_Color color;
    color.r = 0;
    color.g = 0;
    color.b = 0;

    Uint32 pixel = get_pixel(img, x, y);
    SDL_GetRGB(pixel, img->format, &(color.r), &(color.g), &(color.b));
    return color;
}

