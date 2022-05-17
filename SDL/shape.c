#include "shape.h"

SDL_Surface* make_fill_square(SDL_Surface* img, int x1, int y1, int x2, int y2, SDL_Color new_color)
{
    if (img == NULL)
    {
        printf("Failure make_fill_square(): img == NULL\n");
        return img;
    }
    /*

    if (x1 < 0 || x2 < 0 || x1 >= img->w || x2 >= img->w)
    {
        printf("Failure make_fill_square(): x out of bounds\n");
        //return img;
    }
    if (y1 < 0 || y2 < 0 || y1 >= img->h || y2 >= img->h)
    {
        printf("Failure make_fill_square(): y out of bounds\n");
        //return img;
    }

    */

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
            if (x >= 0 && x < img->w && y >= 0 && y < img->h)
            {
                pixel = SDL_MapRGB(img->format, new_color.r, new_color.g, new_color.b);
                put_pixel(img, x, y, pixel);
            }
        }
    }
    return img;
}

void make_horizontal(SDL_Surface* img, int x1, int x2, int y, SDL_Color new_color)
{
    
    if (y < 0 || y >= img->h)
    {
        //printf("Failure make_horizontal(): y out of bounds\n");
        return;
    }

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
        if (x >= 0 && x < img->w)
        {
            pixel = SDL_MapRGB(img->format, new_color.r, new_color.g, new_color.b);
            put_pixel(img, x, y, pixel);
        }
    }
}

void make_vertical(SDL_Surface* img, int y1, int y2, int x, SDL_Color new_color)
{
    if (x < 0 || x >= img->w)
    {
        //printf("Failure make_vertical(): x out of bounds\n");
        return;
    }

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
        if (y >= 0 && y < img->h)
        {
            pixel = SDL_MapRGB(img->format, new_color.r, new_color.g, new_color.b);
            put_pixel(img, x, y, pixel);
        }
    }
}

SDL_Surface* make_empty_square(SDL_Surface* img, int x1, int y1, int x2, int y2, SDL_Color new_color, int size)
{
    if (img == NULL)
    {
        printf("Failure make_empty_square(): img == NULL\n");
        return img;
    }
    /*

    if (x1 < 0 || x2 < 0 || x1 >= img->w || x2 >= img->w)
    {
        printf("Failure make_empty_square(): x out of bounds\n");
        //return img;
    }
    if (y1 < 0 || y2 < 0 || y1 >= img->h || y2 >= img->h)
    {
        printf("Failure make_empty_square(): y out of bounds\n");
        //return img;
    }

    */

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


SDL_Surface* make_empty_triangle(SDL_Surface* img, int x1, int y1, int x2, int y2, SDL_Color new_color, int size)
{
    if (img == NULL)
    {
        printf("Failure make_empty_triangle(): img == NULL\n");
        return img;
    }
    /*

    if (x1 < 0 || x2 < 0 || x1 >= img->w || x2 >= img->w)
    {
        printf("Failure make_empty_triangle(): x out of bounds\n");
        return img;
    }
    if (y1 < 0 || y2 < 0 || y1 >= img->h || y2 >= img->h)
    {
        printf("Failure make_empty_triangle(): y out of bounds\n");
        return img;
    }

    */

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

SDL_Surface* make_fill_triangle(SDL_Surface* img, int x1, int y1, int x2, int y2, SDL_Color new_color, int size)
{
    if (img == NULL)
        errx(EXIT_FAILURE, "Failure make_empty_triangle(): img == NULL");
    if (x1 < 0 || x2 < 0 || x1 >= img->w || x2 >= img->w)
         errx(EXIT_FAILURE, "Failure make_empty_triangle(): x out of bounds");
    if (y1 < 0 || y2 < 0 || y1 >= img->h || y2 >= img->h)
         errx(EXIT_FAILURE, "Failure make_empty_triangle(): y out of bounds");

    int x3 = 0;
    int test;
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
	test = 1;
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
	test = 0;
    }
    //printf("x1:%i y1:%i x2:%i y2:%i x3:%i\n", x1, y1, x2, y2, x3);
    
    while((test && x1 < x2) || (!test && x1 > x2)){
    	drawline(img, new_color, x1, y1, x2, y1, size);
    	drawline(img, new_color, x2, y1, x3, y2, size);
    	drawline(img, new_color, x3, y2, x1, y1, size);
	if(test){
		x1++;
		x2--;
	}
	else{
		x1--;
		x2++;
	}
	if(y1 < y2){
		y1++;
		y2--;
	}
	else{
		y1--;
		y2++;
	}
    }
    return img;
}

// Bresenham Circle
SDL_Surface* bresenham_circle_save(SDL_Surface* img, int x1, int y1, int x2, int y2, SDL_Color color, int size)
{
    if (img == NULL)
    {
        printf("Failure bresenham_circle(): img == NULL\n");
        return img;
    }
    /*
    
    if (x1 < 0 || x2 < 0 || x1 >= img->w || x2 >= img->w)
    {
        printf("Failure bresenham_circle(): x out of bounds\n");
        //return img;
    }
    if (y1 < 0 || y2 < 0 || y1 >= img->h || y2 >= img->h)
    {
        printf("Failure bresenham_circle(): y out of bounds\n");
        //return img;
    }

    */

    int x = 0 , y = sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2)), d = 3 - 2 * y;
    point(img, color, x1 + x, y1 + y, size);
    point(img, color, x1 - x, y1 + y, size);
    point(img, color, x1 + x, y1 - y, size);
    point(img, color, x1 - x, y1 - y, size);
    point(img, color, x1 + y, y1 + x, size);
    point(img, color, x1 - y, y1 + x, size);
    point(img, color, x1 + y, y1 - x, size);
    point(img, color, x1 - y, y1 - x, size);

    while(y >= x){
	    x++;
	    if(d > 0){
		    y--;
		    d = d + 4*(x - y) + 10;
	    }
	    else
		    d = d + 4*x + 6;
        
            point(img, color, x1 + x, y1 + y, size);
            point(img, color, x1 - x, y1 + y, size);
            point(img, color, x1 + x, y1 - y, size);
            point(img, color, x1 - x, y1 - y, size);
            point(img, color, x1 + y, y1 + x, size);
            point(img, color, x1 - y, y1 + x, size);
            point(img, color, x1 + y, y1 - x, size);
            point(img, color, x1 - y, y1 - x, size);
        }
    return img;
}

// Andres Circle
SDL_Surface* bresenham_circle(SDL_Surface* img, int x1, int y1, int x2, int y2, SDL_Color color, int size)
{
    if (img == NULL)
    {
        printf("Failure bresenham_circle(): img == NULL\n");
        return img;
    }

    int x = 0;
    int r = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
    int y = r;
    int d = r - 1;
    while (y >= x)
    {
        point(img, color, x1 + x, y1 + y, size);
        point(img, color, x1 - x, y1 + y, size);
        point(img, color, x1 + x, y1 - y, size);
        point(img, color, x1 - x, y1 - y, size);
        point(img, color, x1 + y, y1 + x, size);
        point(img, color, x1 - y, y1 + x, size);
        point(img, color, x1 + y, y1 - x, size);
        point(img, color, x1 - y, y1 - x, size);

        if (d >= 2 * x)
        {
            d = d - 2 * x - 1;
            x += 1;
        }
        else
        {
            if (d < 2 * (r - y))
            {
                d = d + 2 * y - 1;
                y -= 1;
            }
            else
            {
                d = d + 2 * (y - x - 1);
                y -= 1;
                x += 1;
            }
        }
    }

    return img;
}

SDL_Surface* bresenham_fill_circle(SDL_Surface* img, int x1, int y1, int x2, int y2, SDL_Color color, int size){
	int test = x1 < x2;
	int test2 = y1 < y2;
	while(((test && x1 < x2) || (!test && x1 > x2)) && ((test && x1 < x2) || (!test && x1 > x2))){
		
		img = bresenham_circle(img, x1, y1, x2, y2, color, size);

		if(test){
                	x1++;
                	x2--;
        	}
        	else{
                	x1--;
                	x2++;
        	}
        	if(test2){
                	y1++;
                	y2--;
        	}
        	else{
                	y1--;
                	y2++;
        	}
	}

	return img;
}
