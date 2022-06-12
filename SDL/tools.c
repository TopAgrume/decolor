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
    //printf("thresh: %i\n", threshold);
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
                if (new_color.r != color2.r || new_color.g != color2.g || new_color.b != color2.b)
                {
                    if (compare(color.r, color2.r, threshold) && compare(color.g, color2.g, threshold) && compare(color.b, color2.b, threshold)) 
                    {
                        pixel = SDL_MapRGB(img->format, new_color.r, new_color.g, new_color.b);
                        put_pixel(img, neighbour_x, dequeue_y, pixel);
                        shared_queue_push(seal_queue, pixel, neighbour_x, dequeue_y);
                    }
                }
                //else
                //printf("Rejet\n");
            }
            if (0 <= neighbour_y && neighbour_y < img->h)
            {
                pixel = get_pixel(img, dequeue_x, neighbour_y);
                SDL_GetRGB(pixel, img->format, &(color2.r), &(color2.g), &(color2.b));
                if (new_color.r != color2.r || new_color.g != color2.g || new_color.b != color2.b)
                {
                    if (compare(color.r, color2.r, threshold) && compare(color.g, color2.g, threshold) && compare(color.b, color2.b, threshold)) 
                    {
                        pixel = SDL_MapRGB(img->format, new_color.r, new_color.g, new_color.b);
                        put_pixel(img, dequeue_x, neighbour_y, pixel);
                        shared_queue_push(seal_queue, pixel, dequeue_x, neighbour_y);
                    }
                }
                //else
                //printf("Rejet\n");
            }
        }

        //printf("seal_queue->size: %li\n", seal_queue->size);
        //if (seal_queue->size > 100000)
        //    break;
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
void point_save(SDL_Surface* surface, SDL_Color color, int x, int y, int size)
{
    if (surface == NULL)
        return;
    //if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)
    //    return;

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


void draw(SDL_Surface* img, SDL_Color color, int x1, int y1, int x2, int y2)
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
            if (0 <= x && x + 1 < img->w && 0 <= y && y < img->h)
            {
                put_pixel(img, x, y, SDL_MapRGB(img->format, color.r, color.g, color.b));
                put_pixel(img, x + 1, y, SDL_MapRGB(img->format, color.r, color.g, color.b));
            }
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
            if (0 <= x && x + 1 < img->w && 0 <= y && y < img->h)
            {
                put_pixel(img, x, y, SDL_MapRGB(img->format, color.r, color.g, color.b));
                put_pixel(img, x + 1, y, SDL_MapRGB(img->format, color.r, color.g, color.b));
            }
        }
    }
    if (0 <= x1 && x1 + 1 < img->w && 0 <= y1 && y1 < img->h)
    {
        put_pixel(img, x1, y1, SDL_MapRGB(img->format, color.r, color.g, color.b));
        put_pixel(img, x1 + 1, y1, SDL_MapRGB(img->format, color.r, color.g, color.b));
    }
    if (0 <= x2 && x2 + 1 < img->w && 0 <= y2 && y2 < img->h)
    {
        put_pixel(img, x2, y2, SDL_MapRGB(img->format, color.r, color.g, color.b));
        put_pixel(img, x2 + 1, y2, SDL_MapRGB(img->format, color.r, color.g, color.b));
    }

}



void ligneHorizontale(SDL_Surface* surface, int x, int y, int w, SDL_Color coul)
{
    Uint32 test = SDL_MapRGB(surface->format, coul.r, coul.g, coul.b);
    SDL_Rect r;

    r.x = x;
    r.y = y;
    r.w = w;
    r.h = 1;

    SDL_FillRect(surface, &r, test);
}

void point(SDL_Surface* surface, SDL_Color coul, int cx, int cy, int rayon, int crayon)
{
    if (crayon == 0)
    {
        int d, y, x;

        d = 3 - (2 * rayon);
        x = 0;
        y = rayon;

        while (y >= x) {
            ligneHorizontale(surface, cx - x, cy - y, 2 * x + 1, coul);
            ligneHorizontale(surface, cx - x, cy + y, 2 * x + 1, coul);
            ligneHorizontale(surface, cx - y, cy - x, 2 * y + 1, coul);
            ligneHorizontale(surface, cx - y, cy + x, 2 * y + 1, coul);

            if (d < 0)
                d = d + (4 * x) + 6;
            else {
                d = d + 4 * (x - y) + 10;
                y--;
            }

            x++;
        }
    }
    else if (crayon == 2)
    {
        point_save(surface, coul, cx, cy, rayon);
    }
    else if (crayon == 3)
    {
        draw(surface, coul, cx + rayon / 2, cy - rayon, cx - rayon / 2, cy + rayon);
    }
    else if (crayon == 1)
    {
        for(int i = SDL_max(0, cx - rayon); i < SDL_min(surface->w, cx + rayon); i++){
            make_vertical(surface, SDL_max(0, cy - rayon), SDL_min(surface->h, cy + rayon), i, coul);
        }
    }
    else if (crayon == 4)
    {
        SDL_Rect rec;
        Uint32 bg = SDL_MapRGB(surface->format, coul.r, coul.g, coul.b);
        rec.x = cx - rayon;
        rec.y = cy;
        rec.w = rayon * 2;
        rec.h = 1;
        SDL_FillRect(surface, &rec, bg);

        rec.x = cx;
        rec.y = cy - rayon;
        rec.w = 1;
        rec.h = rayon * 2;
        SDL_FillRect(surface, &rec, bg);
        draw(surface, coul, (cx - rayon / 1.4) , (cy - rayon / 1.4), (cx + rayon / 1.4), (cy + rayon / 1.4));
        draw(surface, coul, (cx + rayon / 1.4) , (cy - rayon / 1.4), (cx - rayon / 1.4), (cy + rayon / 1.4));

    }
    else if (crayon == 5)
    {
        SDL_Rect rec;
        Uint32 bg = SDL_MapRGB(surface->format, coul.r, coul.g, coul.b);
        rec.x = cx - rayon;
        rec.y = cy;
        rec.w = rayon * 2;
        rec.h = 1;
        SDL_FillRect(surface, &rec, bg);

    }
    else
    {
        SDL_Rect rec;
        Uint32 bg = SDL_MapRGB(surface->format, coul.r, coul.g, coul.b);
        rec.x = cx;
        rec.y = cy - rayon;
        rec.w = 1;
        rec.h = rayon * 2;
        SDL_FillRect(surface, &rec, bg);
    }
}

//Draw a line of point() between the two coordinates
// -> 'surface' the pointer on SDL_Surface
// -> 'x1' and 'y1' coordinates of the begin
// -> 'x2' and 'y2' coordinates of the end
// -> 'size' the size of each points
void drawline(SDL_Surface* img, SDL_Color color, int x1, int y1, int x2, int y2, int size, int crayon)
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
            point(img, color, x, y, size, crayon);
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
            point(img, color, x, y, size, crayon);
        }
    }
    point(img, color, x1, y1, size, crayon);
    point(img, color, x2, y2, size, crayon);
}



//This function returns a new surface which is the surface cropped
// -> 'surface' the pointer on SDL_Surface
// -> 'x' and 'y' coordinates of the begin of the square
// -> 'width' and 'height' the width/height of the square
SDL_Surface* crop(SDL_Surface* surface, int x, int y, int width, int height){
    if (x < 0)
        x = 0;
    if (x >= surface->w)
        x = surface->w - 1;
    if (y < 0)
        y = 0;
    if (y >= surface->h)
        y = surface->h - 1;

    if (width < 0)
        width = 0;
    if (width >= surface->w)
        width = surface->w - 1;
    if (height >= surface->h)
        height = surface->h - 1;
    if (height < 0)
        height = 0;


    SDL_Surface* crop;    
    Uint8 r, g, b;
    Uint32 pixel;

    if (x < width && y < height)
    {
        crop = SDL_CreateRGBSurface(SDL_HWSURFACE, width - x, height - y, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
        for(int i = 0; i < width - x; i++){
            for(int j = 0; j < height - y; j++){
                pixel = get_pixel(surface, i + x, j + y);
                SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                pixel = SDL_MapRGB(surface->format, r, g, b);
                put_pixel(crop, i, j, pixel);
            }
        }   
    }
    else
    { 
        if (x < width && y >= height)
        {
            crop = SDL_CreateRGBSurface(SDL_HWSURFACE, width - x, y - height, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
            for(int i = 0; i < width - x; i++){
                for(int j = 0; j < y - height; j++){
                    pixel = get_pixel(surface, i + x, j + height);
                    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                    pixel = SDL_MapRGB(surface->format, r, g, b);
                    put_pixel(crop, i, j, pixel);
                }
            }   
        }
        else
        {
            if (x >= width && y < height)
            {
                crop = SDL_CreateRGBSurface(SDL_HWSURFACE, x - width, height - y, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
                for(int i = 0; i < x - width; i++){
                    for(int j = 0; j < height - y; j++){
                        pixel = get_pixel(surface, i + width, j + y);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        pixel = SDL_MapRGB(surface->format, r, g, b);
                        put_pixel(crop, i, j, pixel);
                    }   
                }   
            }
            else
            {
                crop = SDL_CreateRGBSurface(SDL_HWSURFACE, x - width, y - height, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
                for(int i = 0; i < x - width; i++){
                    for(int j = 0; j < y - height; j++){
                        pixel = get_pixel(surface, i + width, j + height);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        pixel = SDL_MapRGB(surface->format, r, g, b);
                        put_pixel(crop, i, j, pixel);
                    }
                }   
            }
        }
    }
    SDL_FreeSurface(surface);
    return crop;
}

SDL_Surface* rotate(SDL_Surface* surface, int sens)
{
    SDL_Surface* turn = SDL_CreateRGBSurface(SDL_HWSURFACE, surface->h, surface->w, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);

    Uint8 r, g, b;
    Uint32 pixel;

    if (sens)
    {
        for(int i = 0; i < surface->w; i++){
            for(int j = 0; j < surface->h; j++){
                pixel = get_pixel(surface, i, j);
                SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                pixel = SDL_MapRGB(surface->format, r, g, b);
                put_pixel(turn, turn->w - j - 1, i, pixel);
            }
        }
    }
    else
    {
        for(int i = 0; i < surface->w; i++){
            for(int j = 0; j < surface->h; j++){
                pixel = get_pixel(surface, i, j);
                SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                pixel = SDL_MapRGB(surface->format, r, g, b);
                put_pixel(turn, j, turn->h - i - 1, pixel);
            }
        }

    }

    SDL_FreeSurface(surface);
    return turn;
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

    SDL_FreeSurface(surface);
    return reverse;
}


void draw_img(SDL_Surface* surface, SDL_Surface* img, int x1, int y1, int x2, int y2)
{
    int x,y;
    int Dx,Dy;
    int xincr,yincr;
    int erreur;
    int i;
    Uint8 r,g,b;
    Uint32 pixel;

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
            if (0 <= x && x + 1 < img->w && 0 <= y && y < img->h)
            {
                pixel = get_pixel(img, x, y);
                SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                put_pixel(surface, x, y, SDL_MapRGB(surface->format, r, g, b));

                pixel = get_pixel(img, x + 1, y);
                SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                put_pixel(surface, x + 1, y, SDL_MapRGB(surface->format, r, g, b));
            }
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
            if (0 <= x && x + 1 < img->w && 0 <= y && y < img->h)
            {
                pixel = get_pixel(img, x, y);
                SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                put_pixel(surface, x, y, SDL_MapRGB(surface->format, r, g, b));

                pixel = get_pixel(img, x + 1, y);
                SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                put_pixel(surface, x + 1, y, SDL_MapRGB(surface->format, r, g, b));

            }
        }
    }
    if (0 <= x1 && x1 + 1 < img->w && 0 <= y1 && y1 < img->h)
    {
        pixel = get_pixel(img, x1, y1);
        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
        put_pixel(surface, x1, y1, SDL_MapRGB(surface->format, r, g, b));

        pixel = get_pixel(img, x1 + 1, y1);
        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
        put_pixel(surface, x1 + 1, y1, SDL_MapRGB(surface->format, r, g, b));
    }
    if (0 <= x2 && x2 + 1 < img->w && 0 <= y2 && y2 < img->h)
    {
        pixel = get_pixel(img, x2, y2);
        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
        put_pixel(surface, x2, y2, SDL_MapRGB(surface->format, r, g, b));

        pixel = get_pixel(img, x2 + 1, y2);
        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
        put_pixel(surface, x2 + 1, y2, SDL_MapRGB(surface->format, r, g, b));
    }

}



void ligneHorizontale_img(SDL_Surface* surface, int x, int y, int w, SDL_Surface* img)
{
    if (0 <= y && y < surface->h)
    {
        Uint32 pixel;
        Uint8 r, g, b;
        for (int i = x; i < x + w; i++)
        {
            if (0 <= i && i < surface->w)
            {
                pixel = get_pixel(img, i, y);
                SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                put_pixel(surface, i, y, SDL_MapRGB(surface->format, r, g, b));
            }
        }
    }
}

void ligneVertical_img(SDL_Surface* surface, int x, int y, int w, SDL_Surface* img)
{
    if (0 <= x && x < surface->w)
    {
        Uint32 pixel;
        Uint8 r, g, b;
        for (int i = y; i < y + w; i++)
        {
            if (0 <= i && i < surface->h)
            {
                pixel = get_pixel(img, x, i);
                SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                put_pixel(surface, x, i, SDL_MapRGB(surface->format, r, g, b));
            }
        }
    }
}



//Fonctions supplémentaires que l'on supprimera sûrement
void point_image(SDL_Surface* surface, SDL_Surface* img, int x, int y, int size, int crayon)
{
    if (surface == NULL)
        return;

    if (crayon == 0)
    {
        int d, y1, x1;

        d = 3 - (2 * size);
        x1 = 0;
        y1 = size;

        while (y1 >= x1) {
            ligneHorizontale_img(surface, x - x1, y - y1, 2 * x1 + 1, img);
            ligneHorizontale_img(surface, x - x1, y + y1, 2 * x1 + 1, img);
            ligneHorizontale_img(surface, x - y1, y - x1, 2 * y1 + 1, img);
            ligneHorizontale_img(surface, x - y1, y + x1, 2 * y1 + 1, img);

            if (d < 0)
                d = d + (4 * x1) + 6;
            else {
                d = d + 4 * (x1 - y1) + 10;
                y1--;
            }

            x1++;
        }

    }
    else if (crayon == 1)
    {
        Uint8 r,g,b;
        Uint32 pixel;

        int borne1_x = SDL_max(0, x - size);
        int borne2_x = SDL_min(img->w, x + size);
        int borne1_y = SDL_max(0, y - size);
        int borne2_y = SDL_min(img->h, y + size);


        for(int i = borne1_x; i < borne2_x; i++)
        {
            for(int j = borne1_y; j < borne2_y; j++)
            {
                pixel = get_pixel(img, i, j);
                SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                put_pixel(surface, i, j, SDL_MapRGB(surface->format, r, g, b));
            }
        }


    }
    else if (crayon == 2)
    {
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

    else if (crayon == 3)
    {
        draw_img(surface, img, x + size / 2, y - size, x - size / 2, y + size);
    }

    else if (crayon == 4)
    {
        ligneHorizontale_img(surface, x - size, y, 2 * size, img);
        ligneVertical_img(surface, x, y - size, 2 * size, img);
        draw_img(surface, img, (x - size / 1.4) , (y - size / 1.4), (x + size / 1.4), (y + size / 1.4));
        draw_img(surface, img, (x + size / 1.4) , (y - size / 1.4), (x - size / 1.4), (y + size / 1.4));
    }
    else if (crayon == 5)
    {
        ligneHorizontale_img(surface, x - size, y, 2 * size, img);
    }
    else
    {
        ligneVertical_img(surface, x, y - size, 2 * size, img);
    }
}

void drawline_image(SDL_Surface* img, SDL_Surface* img2,  int x1, int y1, int x2, int y2, int size, int crayon)
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
            point_image(img, img2, x, y, size, crayon);
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
            point_image(img, img2, x, y, size, crayon);
        }
    }
    point_image(img, img2, x1, y1, size, crayon);
    point_image(img, img2, x2, y2, size, crayon);
}


SDL_Surface* resize_image(SDL_Surface* surface, int x, int y, int position)
{
    if (x == surface->w && y == surface->h)
        return surface;

    SDL_Surface* img = SDL_CreateRGBSurface(SDL_HWSURFACE, x, y, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);

    int start_x = 0;
    int start_y = 0;
    int borne1_x = 0;
    int borne2_x = x;
    int borne1_y = 0;
    int borne2_y = y;
    int thresh_x, thresh_y;

    if (x < surface->w)
        start_x = (surface->w - x) / 2;
    else
    {
        borne1_x = (x - surface->w) / 2;
        borne2_x = x - borne1_x - 1; //(x % 2);
    }
    if (y < surface->h)
        start_y = (surface->h - y) / 2;
    else
    {
        borne1_y = (y - surface->h) / 2;
        borne2_y = y - borne1_y - 1; //(y % 2);
    }

    SDL_Rect rec;
    rec.x = 0;
    rec.y = 0;
    rec.w = x;
    rec.h = y;
    Uint32 bg = SDL_MapRGB(surface->format, 255, 255, 255);
    SDL_FillRect(img, &rec, bg);

    Uint32 pixel;
    Uint8 r,g,b;
    switch(position)
    {
        case 1: //GOOD
            {
                thresh_x = SDL_min(x, surface->w);
                thresh_y = SDL_min(y, surface->h);
                for(int i = 0; i < thresh_x; i++)
                {
                    for(int j = 0; j < thresh_y; j++)
                    {
                        pixel = get_pixel(surface, i, j);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        pixel = SDL_MapRGB(surface->format, r, g, b);
                        put_pixel(img, i, j, pixel);
                    }
                }
                break;
            }
        case 2: //GOOD
            {
                thresh_y = SDL_min(y, surface->h);
                for(int i = borne1_x; i < borne2_x; i++)
                {
                    for(int j = 0; j < thresh_y; j++)
                    {
                        pixel = get_pixel(surface, start_x + i - borne1_x, j);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        pixel = SDL_MapRGB(surface->format, r, g, b);
                        put_pixel(img, i, j, pixel);
                    }
                }
                break;
            }
        case 3: //GOOD
            {
                thresh_x = surface->w - SDL_min(x, surface->w);
                thresh_y = SDL_min(y, surface->h);

                for(int i = surface->w - 1; i >= thresh_x; i--)
                {
                    for(int j = 0; j < thresh_y; j++)
                    {
                        pixel = get_pixel(surface, i , j);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        pixel = SDL_MapRGB(surface->format, r, g, b);
                        put_pixel(img, img->w + (i - surface->w), j, pixel);
                    }
                }
                break;
            }
        case 4: //GOOD
            {
                thresh_x = SDL_min(x, surface->w);
                for(int i = 0; i < thresh_x; i++)
                {
                    for(int j = borne1_y; j < borne2_y; j++)
                    {
                        pixel = get_pixel(surface, i, start_y + j - borne1_y);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        pixel = SDL_MapRGB(surface->format, r, g, b);
                        put_pixel(img, i, j, pixel);
                    }
                }
                break;
            }
        case 5: //GOOD
            {
                for(int i = borne1_x; i < borne2_x; i++)
                {
                    for(int j = borne1_y; j < borne2_y; j++)
                    {
                        pixel = get_pixel(surface, start_x + i - borne1_x, start_y + j - borne1_y);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        pixel = SDL_MapRGB(surface->format, r, g, b);
                        put_pixel(img, i, j, pixel);
                    }
                }
                break;
            }
        case 6: //GOOD
            {
                thresh_x = surface->w - SDL_min(x, surface->w);
                for(int i = surface->w - 1; i >= thresh_x; i--)
                {
                    for(int j = borne1_y; j < borne2_y; j++)
                    {
                        pixel = get_pixel(surface, i, start_y + j - borne1_y);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        pixel = SDL_MapRGB(surface->format, r, g, b);
                        put_pixel(img, img->w + (i - surface->w ), j, pixel);
                    }
                }
                break;
            }
        case 7: //GOOD
            {
                thresh_x = SDL_min(x, surface->w);
                thresh_y = surface->h - SDL_min(y, surface->h);
                for(int i = 0; i < thresh_x; i++)
                {
                    for(int j = surface->h - 1; j >= thresh_y; j--)
                    {
                        pixel = get_pixel(surface, i, j);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        pixel = SDL_MapRGB(surface->format, r, g, b);
                        put_pixel(img, i, img->h + (j - surface->h), pixel);
                    }
                }
                break;
            }
        case 8: //GOOD
            {
                thresh_y = surface->h - SDL_min(y, surface->h);
                for(int i = borne1_x; i < borne2_x; i++)
                {
                    for(int j = surface->h - 1; j >= thresh_y; j--)
                    {
                        pixel = get_pixel(surface, start_x + i - borne1_x, j);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        pixel = SDL_MapRGB(surface->format, r, g, b);
                        put_pixel(img, i, img->h + (j - surface->h), pixel);
                    }
                }
                break;
            }
        case 9: //GOOD
            {
                thresh_x = surface->w - SDL_min(x, surface->w);
                thresh_y = surface->h - SDL_min(y, surface->h);
                for(int i = surface->w - 1; i >= thresh_x; i--)
                {
                    for(int j = surface->h - 1; j >= thresh_y; j--)
                    {
                        pixel = get_pixel(surface, i, j);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        pixel = SDL_MapRGB(surface->format, r, g, b);
                        put_pixel(img, img->w + (i - surface->w), img->h + (j - surface->h), pixel);
                    }
                }
                break;
            }
        default:
            printf("Default: Resize function\n");
            return surface;
    }

    SDL_FreeSurface(surface);
    return img;
}

//This function returns a new surface which is the surface cropped
// -> 'surface' the pointer on SDL_Surface
// -> 'x' and 'y' coordinates of the begin of the square
// -> 'width' and 'height' the width/height of the square
SDL_Surface* copy_selection(SDL_Surface* surface, int x, int y, int width, int height){
    if (x < 0)
        x = 0;
    if (x >= surface->w)
        x = surface->w - 1;
    if (y < 0)
        y = 0;
    if (y >= surface->h)
        y = surface->h - 1;

    if (width < 0)
        width = 0;
    if (width >= surface->w)
        width = surface->w - 1;
    if (height >= surface->h)
        height = surface->h - 1;
    if (height < 0)
        height = 0;


    SDL_Surface* crop;    
    Uint8 r, g, b;
    Uint32 pixel;

    if (x < width && y < height)
    {
        crop = SDL_CreateRGBSurface(SDL_HWSURFACE, width - x, height - y, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
        for(int i = 0; i < width - x; i++){
            for(int j = 0; j < height - y; j++){
                pixel = get_pixel(surface, i + x, j + y);
                SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                pixel = SDL_MapRGB(surface->format, r, g, b);
                put_pixel(crop, i, j, pixel);
            }
        }   
    }
    else
    { 
        if (x < width && y >= height)
        {
            crop = SDL_CreateRGBSurface(SDL_HWSURFACE, width - x, y - height, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
            for(int i = 0; i < width - x; i++){
                for(int j = 0; j < y - height; j++){
                    pixel = get_pixel(surface, i + x, j + height);
                    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                    pixel = SDL_MapRGB(surface->format, r, g, b);
                    put_pixel(crop, i, j, pixel);
                }
            }   
        }
        else
        {
            if (x >= width && y < height)
            {
                crop = SDL_CreateRGBSurface(SDL_HWSURFACE, x - width, height - y, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
                for(int i = 0; i < x - width; i++){
                    for(int j = 0; j < height - y; j++){
                        pixel = get_pixel(surface, i + width, j + y);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        pixel = SDL_MapRGB(surface->format, r, g, b);
                        put_pixel(crop, i, j, pixel);
                    }   
                }   
            }
            else
            {
                crop = SDL_CreateRGBSurface(SDL_HWSURFACE, x - width, y - height, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
                for(int i = 0; i < x - width; i++){
                    for(int j = 0; j < y - height; j++){
                        pixel = get_pixel(surface, i + width, j + height);
                        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
                        pixel = SDL_MapRGB(surface->format, r, g, b);
                        put_pixel(crop, i, j, pixel);
                    }
                }   
            }
        }
    }
    return crop;
}

void past_selection(SDL_Surface* img, SDL_Surface* surface, int x, int y)
{
    if (surface == NULL)
        return;

    Uint8 r, g, b;
    Uint32 pixel;
    /*
       for(int i = x; i < img->w && i < x + surface->w; i++){
       for(int j = y; j < img->h && j < y + surface->h; j++){
       pixel = get_pixel(surface, i - x, j - y);
       SDL_GetRGB(pixel, surface->format, &r, &g, &b);
       pixel = SDL_MapRGB(surface->format, r, g, b);
       put_pixel(img, i, j, pixel);
       }
       }*/
    int start_x = 0;
    int start_y = 0;
    if (x < surface->w / 2)
        start_x = surface->w / 2 - x;
    if (y < surface->h / 2)
        start_y = surface->h / 2 - y;


    int borne1_x = SDL_max(0, x - surface->w / 2);
    int borne2_x = SDL_min(img->w, x + surface->w / 2);
    int borne1_y = SDL_max(0, y - surface->h / 2);
    int borne2_y = SDL_min(img->h, y + surface->h / 2);


    for(int i = borne1_x; i < borne2_x; i++)
    {
        for(int j = borne1_y; j < borne2_y; j++)
        {
            pixel = get_pixel(surface, start_x + i - borne1_x, start_y + j - borne1_y);
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            pixel = SDL_MapRGB(surface->format, r, g, b);
            put_pixel(img, i, j, pixel);
        }
    }

}

void erase_selection(SDL_Surface* surface, int x, int y, int width, int height){
    if (x < 0)
        x = 0;
    if (x >= surface->w)
        x = surface->w - 1;
    if (y < 0)
        y = 0;
    if (y >= surface->h)
        y = surface->h - 1;

    if (width < 0)
        width = 0;
    if (width >= surface->w)
        width = surface->w;
    if (height >= surface->h)
        height = surface->h;
    if (height < 0)
        height = 0;

    SDL_Rect rec;
    Uint32 bg = SDL_MapRGB(surface->format, 255, 255, 255);

    if (x < width && y < height)
    { 
        rec.x = x;
        rec.y = y;
        rec.w = width - x;
        rec.h = height - y;
    }
    else
    { 
        if (x < width && y >= height)
        {
            rec.x = x;
            rec.y = height;
            rec.w = width - x;
            rec.h = y - height;
        }
        else
        {
            if (x >= width && y < height)
            {
                rec.x = width;
                rec.y = y;
                rec.w = x - width;
                rec.h = height - y;

            }
            else
            {
                rec.x = width;
                rec.y = height;
                rec.w = x - width;
                rec.h = y - height;

            }
        }
    }

    SDL_FillRect(surface, &rec, bg);
}


