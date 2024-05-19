#include "maze.h"

/**
 * close_instance - Function to release SDL instance
 * @instance: Running SDL instance
 * Return: empty
*/

void close_instance(SDL_Instance *instance)
{
	//Deallocate surfaces
	for( int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i )
	{
		SDL_FreeSurface( gKeyPressSurfaces[ i ] );
		gKeyPressSurfaces[ i ] = NULL;
	}

	SDL_DestroyRenderer(instance->renderer);
	SDL_DestroyWindow(instance->window);
	SDL_Quit();
	free(instance);
}
