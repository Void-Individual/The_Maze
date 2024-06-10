#include "maze.h"

/**
 * close - Function to close SDL
 * Return: empty
*/

void close(void)
{
    // Dealocate Surface;
    SDL_FreeSurface(image_surface);
	image_surface = NULL;

	// Destroy Window;
	SDL_DestroyWindow(window);
	window = NULL;

	// Quit SDL Subsystems
	SDL_Quit();
}
