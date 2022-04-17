#include "shape.h"

void make_horizontal(SDL_Surface* img, int x1, int x2, int y, SDL_Color new_color)
{
    if (y < 0 || y >= img->h)
        errx(EXIT_FAILURE, "Failure make_horizontal(): y out of bounds");

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

    int y;
    Uint32 pixel;
    for (y = y1; y <= y2; y++)
    {
        pixel = SDL_MapRGB(img->format, new_color.r, new_color.g, new_color.b);
        put_pixel(img, x, y, pixel);
    }
}

SDL_Surface* make_square(SDL_Surface* img, int x1, int y1, int x2, int y2, SDL_Color new_color, int size)
{
    if (img == NULL)
        errx(EXIT_FAILURE, "Failure make_square(): img == NULL");
    if (x1 < 0 || x2 < 0 || x1 >= img->w || x2 >= img->w)
         errx(EXIT_FAILURE, "Failure make_square(): x out of bounds");
    if (y1 < 0 || y2 < 0 || y1 >= img->h || y2 >= img->h)
         errx(EXIT_FAILURE, "Failure make_square(): y out of bounds");

    int move_x1 = 0;
    int move_x2 = 0;
    int move_y1 = 0;
    int move_y2 = 0;

    if (x2 > x1)
    {
        if (y1 > y2)
        {
            move_y1 = size;
            move_x2 = size;
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
    // Vertical bar 1
    for (x = x1 - move_x1; x < img->w && x <= x1 + move_x2; x++)
        make_vertical(img, y1, y2, x, new_color);
    
    // Vertical bar 2
    for (x = x2 - move_x2; x < img->w && x <= x2 + move_x1; x++)
        make_vertical(img, y1, y2, x, new_color);

    // Horizontal bar 1
    for (y = y1 - move_y1; y < img->h && y <= y1 + move_y2; y++)
        make_horizontal(img, x1, x2, y, new_color);

    // Horizontal bar 2
    for (y = y2 - move_y2; y < img->h && y <= y2 + move_y1; y++)
        make_horizontal(img, x1, x2, y, new_color);

    return img;
}
