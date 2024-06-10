#include "maze.h"

/**
 * loadSurface - FUnction to load image at a path
 * @path: Path to the image
 * Return: The loaded image
*/

SDL_Surface *loadSurface(char *path)
{
	/*Load image at the specified path*/
	SDL_Surface *loadedsurface = NULL;
	SDL_Surface *optimizedSurface = NULL;

	if (strcmp(path, "../image/texture.png") == 0)
		loadedsurface = IMG_Load(path);
	else
		loadedsurface = SDL_LoadBMP(path);

	if (loadedsurface == NULL)
		printf("Unable to load image %s\nError: %s\n", path, SDL_GetError());
	else /*convert surface to screen format*/
	{
		optimizedSurface = SDL_ConvertSurface(loadedsurface, windowsurface->format, 0);
		if (optimizedSurface == NULL)
			printf("Unable to optimize image %s!\n Error: %s\n", path, SDL_GetError());
		SDL_FreeSurface(loadedsurface); /*Get rid of old loaded surface*/
	}

	return (optimizedSurface);
}

/**
 * loadTexture - Function to load an individual image as texture
 * @path: path to the image
 * Return: the texture
*/

SDL_Texture *loadTexture(char *path)
{
    SDL_Texture* newTexture = NULL; //The final texture
    SDL_Surface* loadedSurface = IMG_Load(path); //Load image at specified path

    if( loadedSurface == NULL )
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if( newTexture == NULL )
            printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}
