#include "maze.h"

/**
 * close_instance - Function to release SDL instance
 * @instance: Running SDL instance
 * Return: empty
*/

void close_instance(SDL_Instance *instance)
{
	SDL_FreeSurface(instance->imagesurface);
	SDL_DestroyRenderer(instance->renderer);
	SDL_DestroyWindow(instance->window);
	SDL_Quit();
	free(instance);
}
