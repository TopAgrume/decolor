#include "tools.h"

// This function test if |color1 - color2| <= treshold
// -> 'color1', 'color2', 'threshold' are integers
int compare(int color1, int color2, int threshold)
{
    if (color1 > color2)
        return (color1 - color2) <= threshold;
    else
        return (color2 - color1) <= threshold;
}

// This function returns the image after using the seal (Format SDL_Surface*)
// -> 'img' the pointer on SDL_Surface (image)
// -> 'x' and 'y' coordinates of the chosen pixel
// -> 'new_color' the color the function use to fill the image
// -> 'threshold' for color filling
SDL_Surface* filling_seal(SDL_Surface* img, int x, int y, SDL_Color new_color, int threshold)
{
    // Initialization
    SDL_Color color;
    color.r = 0;
    color.g = 0;
    color.b = 0;

    // Try the value of the first pixel
    Uint32 pixel = get_pixel(img, x, y);
    SDL_GetRGB(pixel, img->format, &(color.r), &(color.g), &(color.b));
    if (color.r == new_color.r && color.g == new_color.g && color.b == new_color.b)
        return img;
    pixel = SDL_MapRGB(img->format, new_color.r, new_color.g, new_color.b);
    put_pixel(img, x, y, pixel);

    // If colors are different, then apply the seal tool
    shared_queue* seal_queue = shared_queue_new();
    shared_queue_push(seal_queue, pixel, x, y);
    SDL_Color color2;
    color2.r = 0;
    color2.g = 0;
    color2.b = 0;
   

    // BFS of a color on the image
    int dequeue_x = 0;
    int dequeue_y = 0;
    int neighbour_x = 0;
    int neighbour_y = 0;
    int move = 0;
    while (seal_queue->size != 0)
    {
        color2.r = 0;
        color2.g = 0;
        color2.b = 0;
        
        pixel = shared_queue_pop(seal_queue, &dequeue_x, &dequeue_y);
        for (move = -1; move < 2; move += 2)
        {
            neighbour_x = dequeue_x + move;
            neighbour_y = dequeue_y + move;
            if (0 <= neighbour_x && neighbour_x < img->w)
            {
                pixel = get_pixel(img, neighbour_x, dequeue_y);
                SDL_GetRGB(pixel, img->format, &(color2.r), &(color2.g), &(color2.b));
                //if (color.r == color2.r && color.g == color2.g && color.b == color2.b)
                if (compare(color.r, color2.r, threshold) && compare(color.g, color2.g, threshold) && compare(color.b, color2.b, threshold)) 
                {
                    pixel = SDL_MapRGB(img->format, new_color.r, new_color.g, new_color.b);
                    put_pixel(img, neighbour_x, dequeue_y, pixel);
                    shared_queue_push(seal_queue, pixel, neighbour_x, dequeue_y);
                }
                //else
                    //printf("Rejet\n");
            }
            if (0 <= neighbour_y && neighbour_y < img->h)
            {
                pixel = get_pixel(img, dequeue_x, neighbour_y);
                SDL_GetRGB(pixel, img->format, &(color2.r), &(color2.g), &(color2.b));
                //if (color.r == color2.r && color.g == color2.g && color.b == color2.b)
                if (compare(color.r, color2.r, threshold) && compare(color.g, color2.g, threshold) && compare(color.b, color2.b, threshold)) 
                {
                    pixel = SDL_MapRGB(img->format, new_color.r, new_color.g, new_color.b);
                    put_pixel(img, dequeue_x, neighbour_y, pixel);
                    shared_queue_push(seal_queue, pixel, dequeue_x, neighbour_y);
                }
                //else
                    //printf("Rejet\n");
            }
        }
    }
    shared_queue_destroy(seal_queue);
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

void point(SDL_Surface* surface, SDL_Color color, int x, int y, int size){
    int i = x-size;
    int j = y;
    int nb = 0;
    while(i < x){
	    while(j < y+nb){
		    put_pixel(surface, i, j, SDL_MapRGB(surface->format, color.r, color.g, color.b));
		    j++;
	    }
	    i++;
	    nb++;
	    j = y - nb;
    }
    while(i <= x+size){
            while(j < y+nb){
                    put_pixel(surface, i, j, SDL_MapRGB(surface->format, color.r, color.g, color.b));
                    j++;
            }
            i++;
            nb --;
            j = y - nb;
    }
}

void line(SDL_Surface* surface, SDL_Color color, int x1, int y1, int x2, int y2, int size){
    int a;
    if(x2 - x1 > y2 - y1){
	    int i = x1;
	    if(x2 - x1 == 0)
		    a = 0;
	    else
		    a = (y2 - y1) / (x2 - x1);
	    int b = y1 - a * x1;
	    printf("%i\n",size);
	    while(i < x2){
		    point(surface, color, i, a * i + b, size);
		    //put_pixel(surface, i, a * i + b, SDL_MapRGB(surface->format, color.r, color.g, color.b));
		    i++;
	    }
    }
    else{
	    int i = y1;
	    if(y2 - y1 == 0)
                    a = 0;
            else
                    a = (x2 - x1) / (y2 - y1);
            int b = x1 - a * y1;
            printf("%i\n",size);
            while(i < y2){
                    point(surface, color, i, a * i + b, size);
                    //put_pixel(surface, a * i + b, i, SDL_MapRGB(surface->format, color.r, color.g, color.b));
                    i++;
            }
    }
}

//This function create a surface and put a line between each event of left click
// -> 'color' the color of the brush
/*void brush(SDL_Color color){
	SDL_Surface* surface;
	int loop = 1;

	while(loop){

	}
}*/
