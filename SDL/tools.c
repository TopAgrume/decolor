#include "tools.h"

// This function test if |color1 - color2| <= treshold
// -> 'color1', 'color2', 'threshold' are integers
int compare(int color1, int color2, int threshold)
{
    //printf("%i; %i\n", color1 - color2, color2 - color1);
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
        //if (new_color.r != color2.r || new_color.g != color2.g || new_color.b == color2.b)
        for (move = -1; move < 2; move += 2)
        {
            neighbour_x = dequeue_x + move;
            neighbour_y = dequeue_y + move;
            if (0 <= neighbour_x && neighbour_x < img->w)
            {
                pixel = get_pixel(img, neighbour_x, dequeue_y);
                SDL_GetRGB(pixel, img->format, &(color2.r), &(color2.g), &(color2.b));
                //if (new_color.r != color2.r || new_color.g != color2.g || new_color.b == color2.b)
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
                //if (new_color.r != color2.r || new_color.g != color2.g || new_color.b == color2.b)
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
    //printf("seal_queue->size: %li\n", seal_queue->size);

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

//This function create a cross of pixel with the size indicated
// -> 'surface' the pointer on SDL_Surface
// -> 'color' the color of the point (the color can be transparent to be a rubber)
// -> 'x' and 'y' coordinates where the point shall be
// -> 'size' the size of the cross
void point(SDL_Surface* surface, SDL_Color color, int x, int y, int size)
{
    if (surface == NULL)
        return;
    if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)
        return;

    int i = x-size;
    int j = y;
    int nb = 0;
    while(i < x){
	    while(j < y+nb){
            if (i >= 0 && j >= 0 && i < surface->w && j < surface->h)
		        put_pixel(surface, i, j, SDL_MapRGB(surface->format, color.r, color.g, color.b));
		    j++;
	    }
	    i++;
	    nb++;
	    j = y - nb;
    }
    while(i <= x+size){
            while(j < y+nb){
                if (i >= 0 && j >= 0 && i < surface->w && j < surface->h)
                    put_pixel(surface, i, j, SDL_MapRGB(surface->format, color.r, color.g, color.b));
                j++;
            }
            i++;
            nb --;
            j = y - nb;
    }
}

//Draw a line of point() between the two coordinates
// -> 'surface' the pointer on SDL_Surface
// -> 'x1' and 'y1' coordinates of the begin
// -> 'x2' and 'y2' coordinates of the end
// -> 'size' the size of each points
void drawline(SDL_Surface* img, SDL_Color color, int x1, int y1, int x2, int y2, int size)
{
    int x,y;
    int Dx,Dy;
    int xincr,yincr;
    int erreur;
    int i;

    Dx = abs(x2-x1);
    Dy = abs(y2-y1);
    if(x1<x2)
        xincr = 1;
    else
        xincr = -1;
    if(y1<y2)
        yincr = 1;
    else
        yincr = -1;

    x = x1;
    y = y1;
    if(Dx>Dy)
    {
        erreur = Dx/2;
        for(i=0;i<Dx;i++)
        {
            x += xincr;
            erreur += Dy;
            if(erreur>Dx)
            {
                erreur -= Dx;
                y += yincr;
            }
            point(img, color, x, y, size);
        }
    }
    else
    {
        erreur = Dy/2;
        for(i=0;i<Dy;i++)
        {
            y += yincr;
            erreur += Dx;
            if(erreur>Dy)
            {
                erreur -= Dy;
                x += xincr;
            }
            point(img, color, x, y, size);
        }
    }
    point(img, color, x1, y1, size);
    point(img, color, x2, y2, size);
}

/*
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
}*/

//This function create a surface and put a line between each event of left click
// -> 'color' the color of the brush
/*void brush(SDL_Color color){
	SDL_Surface* surface;
	int loop = 1;

	while(loop){

	}
}*/

//This function returns a new surface which is the surface cropped
// -> 'surface' the pointer on SDL_Surface
// -> 'x' and 'y' coordinates of the begin of the square
// -> 'width' and 'height' the width/height of the square
SDL_Surface* crop(SDL_Surface* surface, int x, int y, int width, int height){
    if(x + width > surface->w || y + height > surface->h)
	    errx(EXIT_FAILURE, "Problème au rognage");

    SDL_Surface* crop = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);

    Uint8 r, g, b;
    Uint32 pixel;
    for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                    pixel = get_pixel(surface, i+x, j+y);
                    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                    pixel = SDL_MapRGB(surface->format, r, g, b);
                    put_pixel(crop, i, j, pixel);
            }
    }

    return crop;
}

//This function returns the image but reversed on the x axe or y axe
// -> 'surface' the pointer on SDL_Surface
// -> 'horizontal and 'vertical' are booleans to know the axes
SDL_Surface* reversion(SDL_Surface* surface, int horizontal, int vertical){
    SDL_Surface* reverse = SDL_CreateRGBSurface(SDL_HWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
    Uint8 r, g, b;
    Uint32 pixel;

    if(horizontal && !vertical){
    	    for(int i = 0; i < surface->w; i++){
            	    for(int j = 0; j < surface->h; j++){
                    	    pixel = get_pixel(surface, i, j);
                    	    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                    	    pixel = SDL_MapRGB(surface->format, r, g, b);
                    	    put_pixel(reverse, surface->w - i -1, j, pixel);
            	    }
    	    }
    }

    if(!horizontal && vertical){
	    for(int i = 0; i < surface->w; i++){
		    for(int j = 0; j < surface->h; j++){
                      	    pixel = get_pixel(surface, i, j);
                    	    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                    	    pixel = SDL_MapRGB(surface->format, r, g, b);
                    	    put_pixel(reverse, i, surface->h - j - 1, pixel);
            	   }
    	   }
    }

    if(horizontal && vertical){
            for(int i = 0; i < surface->w; i++){
                    for(int j = 0; j < surface->h; j++){
                            pixel = get_pixel(surface, i, j);
                            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                            pixel = SDL_MapRGB(surface->format, r, g, b);
                            put_pixel(reverse, surface->w - i -1, surface->h - j - 1, pixel);
                    }
            }
    }


    return reverse;
}

//Fonctions supplémentaires que l'on supprimera sûrement

void point_image(SDL_Surface* surface, SDL_Surface* img, int x, int y, int size)
{
    if (surface == NULL)
        return;
    if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)
        return;

    int i = x-size;
    int j = y;
    int nb = 0;
    Uint8 r,g,b;
    Uint32 pixel;
    while(i < x){
            while(j < y+nb){
		    if (i >= 0 && j >= 0 && i < surface->w && j < surface->h){
			    pixel = get_pixel(img, i,j);
			    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
			    put_pixel(surface, i, j, SDL_MapRGB(surface->format, r, g, b));
		    }
                    j++;
            }
            i++;
            nb++;
            j = y - nb;
    }
    while(i <= x+size){
            while(j < y+nb){
                if (i >= 0 && j >= 0 && i < surface->w && j < surface->h){
			pixel = get_pixel(img, i,j);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        put_pixel(surface, i, j, SDL_MapRGB(surface->format, r, g, b));
		}
                j++;
            }
            i++;
            nb --;
            j = y - nb;
    }
}

void drawline_image(SDL_Surface* img, SDL_Surface* img2,  int x1, int y1, int x2, int y2, int size)
{
    int x,y;
    int Dx,Dy;
    int xincr,yincr;
    int erreur;
    int i;

    Dx = abs(x2-x1);
    Dy = abs(y2-y1);
    if(x1<x2)
        xincr = 1;
    else
        xincr = -1;
    if(y1<y2)
        yincr = 1;
    else
        yincr = -1;

    x = x1;
    y = y1;
    if(Dx>Dy)
    {
        erreur = Dx/2;
        for(i=0;i<Dx;i++)
        {
            x += xincr;
            erreur += Dy;
            if(erreur>Dx)
            {
                erreur -= Dx;
                y += yincr;
            }
            point_image(img, img2, x, y, size);
        }
    }
    else
    {
        erreur = Dy/2;
        for(i=0;i<Dy;i++)
        {
            y += yincr;
            erreur += Dx;
            if(erreur>Dy)
            {
                erreur -= Dy;
                x += xincr;
            }
            point_image(img, img2, x, y, size);
	}
    }
    point_image(img, img2, x1, y1, size);
    point_image(img, img2, x2, y2, size);
}

