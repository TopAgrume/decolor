#include "shape.h"

SDL_Surface* make_fill_square(SDL_Surface* img, int x1, int y1, int x2, int y2, SDL_Color new_color)
{
    if (img == NULL)
        errx(EXIT_FAILURE, "Failure make_fill_square(): img == NULL");
    if (x1 < 0 || x2 < 0 || x1 >= img->w || x2 >= img->w)
         errx(EXIT_FAILURE, "Failure make_fill_square(): x out of bounds");
    if (y1 < 0 || y2 < 0 || y1 >= img->h || y2 >= img->h)
         errx(EXIT_FAILURE, "Failure make_fill_square(): y out of bounds");
    int x, y;

    int tmp;

    if (x1 > x2)
    {
        tmp = x1;
        x1 = x2;
        x2 = tmp;

        if (y1 > y2)
        {
            tmp = y1;
            y1 = y2;
            y2 = tmp;
        }
    }
    else
    {
        if (y1 > y2)
        {
            tmp = y1;
            y1 = y2;
            y2 = tmp;
        }
    }

    Uint32 pixel;
    for (x = x1; x <= x2; x++)
    {
        for (y = y1; y <= y2; y++)
        {
            pixel = SDL_MapRGB(img->format, new_color.r, new_color.g, new_color.b);
            put_pixel(img, x, y, pixel);
        }
    }
    return img;
}

void make_horizontal(SDL_Surface* img, int x1, int x2, int y, SDL_Color new_color)
{
    if (y < 0 || y >= img->h)
        errx(EXIT_FAILURE, "Failure make_horizontal(): y out of bounds");
    if (x1 > x2)
    {
        int tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    int x;
    Uint32 pixel;
    for (x = x1; x <= x2; x++)
    {
        pixel = SDL_MapRGB(img->format, new_color.r, new_color.g, new_color.b);
        put_pixel(img, x, y, pixel);
    }
}

void make_vertical(SDL_Surface* img, int y1, int y2, int x, SDL_Color new_color)
{
    if (x < 0 || x >= img->w)
        errx(EXIT_FAILURE, "Failure make_vertical(): x out of bounds");
    if (y1 > y2)
    {
        int tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    int y;
    Uint32 pixel;
    for (y = y1; y <= y2; y++)
    {
        pixel = SDL_MapRGB(img->format, new_color.r, new_color.g, new_color.b);
        put_pixel(img, x, y, pixel);
    }
}

SDL_Surface* make_empty_square(SDL_Surface* img, int x1, int y1, int x2, int y2, SDL_Color new_color, int size)
{
    if (img == NULL)
        errx(EXIT_FAILURE, "Failure make_empty_square(): img == NULL");
    if (x1 < 0 || x2 < 0 || x1 >= img->w || x2 >= img->w)
         errx(EXIT_FAILURE, "Failure make_empty_square(): x out of bounds");
    if (y1 < 0 || y2 < 0 || y1 >= img->h || y2 >= img->h)
         errx(EXIT_FAILURE, "Failure make_empty_square(): y out of bounds");

    int move_x1 = 0;
    int move_x2 = 0;
    int move_y1 = 0;
    int move_y2 = 0;

    if (x2 > x1)
    {
        if (y1 > y2)
        {
            move_x2 = size;
            move_y1 = size;
        }
        else
        {
            move_y2 = size;
            move_x2 = size;
        }
    }
    else
    {
        if (y2 > y1)
        {
            move_y2 = size;
            move_x1 = size;
        }
        else
        {
            move_y1 = size;
            move_x1 = size;
        }
    }

    int x, y;
    //printf("x: %i\n", x1 - move_x1);
    // Vertical bar 1
    for (x = x1 - move_x1; x < img->w && x <= x1 + move_x2; x++)
        make_vertical(img, y1, y2, x, new_color);
    //printf("new x: %i\n", x);

    //printf("x: %i\n", x2 - move_x2);
    // Vertical bar 2
    for (x = x2 - move_x2; x < img->w && x <= x2 + move_x1; x++)
        make_vertical(img, y1, y2, x, new_color);
    //printf("new x: %i\n", x);

    //printf("y: %i\n", y1 - move_y1);
    // Horizontal bar 1
    for (y = y1 - move_y1; y < img->h && y <= y1 + move_y2; y++)
        make_horizontal(img, x1, x2, y, new_color);
    //printf("new y: %i\n", y);

    //printf("y: %i\n", y2 - move_y2);
    // Horizontal bar 2
    for (y = y2 - move_y2; y < img->h && y <= y2 + move_y1; y++)
        make_horizontal(img, x1, x2, y, new_color);
    //printf("new y: %i\n", y);
    //printf("\n");
    return img;
}

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

SDL_Surface* make_empty_triangle(SDL_Surface* img, int x1, int y1, int x2, int y2, SDL_Color new_color, int size)
{
    if (img == NULL)
        errx(EXIT_FAILURE, "Failure make_empty_triangle(): img == NULL");
    if (x1 < 0 || x2 < 0 || x1 >= img->w || x2 >= img->w)
         errx(EXIT_FAILURE, "Failure make_empty_triangle(): x out of bounds");
    if (y1 < 0 || y2 < 0 || y1 >= img->h || y2 >= img->h)
         errx(EXIT_FAILURE, "Failure make_empty_triangle(): y out of bounds");

    //int move_x1 = 0;
    //int move_x2 = 0;
    //int move_y1 = 0;
    //int move_y2 = 0;
    
    int x3 = 0;
    if (x1 < x2)
    {
        if (y1 < y2)
        {
            x3 = x1 + ((x2 - x1) / 2);
            //printf("1\n\n");
        }
        else
        {
            x3 = x1 + ((x2 - x1) / 2);
            //printf("2\n\n");
        }
    }
    else
    {
        if (y1 < y2)
        {
            x3 = x2 + ((x1 - x2) / 2);
            //printf("3\n\n");
        }
        else
        {
            x3 = x2 + ((x1 - x2) / 2);
            //printf("4\n\n");
        }
    }
    //printf("x1:%i y1:%i x2:%i y2:%i x3:%i\n", x1, y1, x2, y2, x3);
    drawline(img, new_color, x1, y1, x2, y1, size);
    drawline(img, new_color, x2, y1, x3, y2, size);
    drawline(img, new_color, x3, y2, x1, y1, size);
    return img;
}
