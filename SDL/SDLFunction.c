#include "SDLFunction.h"

void init_sdl()
{
    // Init only the video part
    // If it fails, die with an error message.
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1, "Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* loadImage(char* path)
{
    SDL_Surface* img;
    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());
    return img;
}


SDL_Surface* displayImage(SDL_Surface* img)
{
    SDL_Surface* screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE | SDL_ANYFORMAT);
    if (screen == NULL)
    {
        errx(1, "Couldn't set %dx%d video mode: %s\n", img->w, img->h, SDL_GetError());
    }
    // Blit onto the screen surface
    if (SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    return screen;
}

SDL_Surface* copy_image(SDL_Surface* img)
{
    Uint32 pixel;
    SDL_Surface* copy;
    copy = SDL_CreateRGBSurface(SDL_HWSURFACE,
        img->w * 2,
        img->h * 2,
        img->format->BitsPerPixel, 0, 0, 0, 0);
    for (int i = 0; i < img->h; i++)
    {
        for (int j = 0; j < img->w; j++)
        {
            pixel = get_pixel(img, i, j);
            put_pixel(copy, i * 2, j * 2, pixel);
        }
    }
    return(copy);
}



// Filter RGB to Gray
void RGB2GRAY(SDL_Surface* image)
{
    for (int x = 0; x < image->w; ++x)
    {
        for (int y = 0; y < image->h; ++y)
        {
            Uint32 pixel = get_pixel(image, x, y);
            Uint8 r = 0;
            Uint8 g = 0;
            Uint8 b = 0;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            r = 0.212671f * r + 0.715160f * g + 0.072169f * b;
            Uint32 newpixel = SDL_MapRGB(image->format, r, r, r);
            put_pixel(image, x, y, newpixel);
        }
    }
}


// Filter RGB to Black
void RGB2BLACK(SDL_Surface* image)
{
    RGB2GRAY(image);
    int thresholdValue = otsu_method(image);
    GRAY2BLACK(image, thresholdValue);
}

int otsu_method(SDL_Surface* img)
{

    int grayLevels = 256;
    //Get the row, columns and size of the image
    int y_size = img->h, x_size = img->w, size = y_size * x_size;


    //Histogram used to store the value of the grey reference for each pixel
    int* hist = get_histogram(img);

    //Needed for the math, in order to calculate the threshold
    double prob[grayLevels], omega[grayLevels];
    double myu[grayLevels]; //Create an array to calculate the mean value
    double max_sigma, sigma[grayLevels]; //Maths, always maths
    int i; //loop variables
    int threshold = -1; //The value we want

    //Initialize the value of the histogram,
    //we never know there can be some random values


    //Creating the prob array with the value from the hist / size
    for (i = 0; i < grayLevels; i++)
    {
        prob[i] = (double)hist[i] / size;
    }

    //Setting the first value because the loop start at 1 in order to do
    //the maths
    omega[0] = prob[0];
    myu[0] = 0.0;

    for (i = 1; i < grayLevels; i++)
    {
        //Doing the maths, see wikipedia otsu method
        omega[i] = omega[i - 1] + prob[i];
        myu[i] = myu[i - 1] + i * prob[i];
    }

    threshold = 0;
    max_sigma = 0.0;
    for (i = 0; i < grayLevels - 1; i++)
    {
        if (omega[i] != 0.0 && omega[i] != 1.0)
            sigma[i] = (myu[grayLevels - 1] * omega[i] - myu[i]) *
            (myu[grayLevels - 1] * omega[i] - myu[i]) / (omega[i] *
                (1.0 - omega[i]));
        else
            sigma[i] = 0.0;
        if (sigma[i] > max_sigma)
        {
            max_sigma = sigma[i];
            threshold = i;
        }


    }

    free(hist);

    printf("\tThreshold Value from Otsu method = %d\n", threshold);
    return threshold;

}