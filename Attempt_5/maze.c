#include "maze.h"

/**
 * main - Functional Key Presses
 * Return: 0
*/

// Define the array of surfaces
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
// Define the current surface
SDL_Surface* gCurrentSurface = NULL;

int main(void)
{
	SDL_Instance *instance; /* Create an SDL instance variable*/
	int quit = 0;

    instance = malloc(sizeof(SDL_Instance));/*Allocate memory to the instance*/
    if (instance == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for SDL_Instance\n");
        return (1);
    }

    if (init_instance(instance) != 0) /*Initialize the SDL instance*/
	{
		free(instance);
		return (1);
	}
	//Load media
	if(!loadMedia())
		printf( "Failed to load media!\n" );

	/**Set default surface*/
	gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
	while (!quit) /*DON'T Set up an infinite loop that you cant break*/
	{
		/*Get the window surface*/
		instance->windowsurface = SDL_GetWindowSurface(instance->window);
		/*Blit the image onto the window surface*/
		quit = poll_events();
		SDL_BlitSurface(gCurrentSurface, NULL, instance->windowsurface, NULL);
		/*Update the surface*/
		SDL_UpdateWindowSurface(instance->window);
		// SDL_RenderClear(instance->renderer);
		//draw_stuff(instance);
		//SDL_RenderPresent(instance->renderer);
	}

	/*Hack to get window to stay up (Repurposed for c instead of c++)*/
    /*SDL_Event e; int quit = 0; while( quit == 0 ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = 1; } }*/

	close_instance(instance); /*Function to release and free the SDL instance*/
    return (0);
}
