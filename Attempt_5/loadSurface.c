#include "maze.h"

/**
 * loadSurface - FUnction to load image at a path
 * @path: Path to the image
 * Return: The loaded image
*/

SDL_Surface *loadSurface(char *path)
{
	/*Load image at the specified path*/
	SDL_Surface *loadedsurface = SDL_LoadBMP(path);
	//SDL_Surface * optimizedSurface = NULL;

	if (loadedsurface == NULL)
		printf("Unable to load image %s\nError: %s\n", path, SDL_GetError());
	//else /*convert surface to screen format*/
	//{
	//	optimizedSurface = SDL_ConvertSurface(loadedsurface,
	//		instance->windowsurface->format, 0);
	//}

	return (loadedsurface);
}
