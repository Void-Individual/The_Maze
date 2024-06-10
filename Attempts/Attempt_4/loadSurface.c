#include "maze.h"

/**
 * loadSurface - FUnction to load image at a path
 * @path: Path to the image
 * Return: The loaded image
*/

SDL_Surface *loadSurface(char *path)
{
	SDL_Surface *loadedsurface = SDL_LoadBMP(path);
	if (loadedsurface == NULL)
		printf("Unable to load image %s\nError: %s\n", path, SDL_GetError());
	return (loadedsurface);
}
