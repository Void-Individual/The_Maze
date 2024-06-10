#include "maze.h"

int loadmedia(void)
{
    int success = 1;

    // Load splash image
    image_surface = SDL_LoadBMP("../image/hello_world.bmp");
    if (image_surface == NULL)
    {
        printf("Unable to load Image %s!\nError: %s", "../image/hello_world.bmp", SDL_GetError());
        success = 0;
    }

    return (success);
}
