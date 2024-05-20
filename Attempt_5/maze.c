#include "maze.h"

/**
 * main - Functional Key Presses
 * Return: 0
*/

// Define the array of surfaces
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
// Define the current surface
SDL_Surface* gCurrentSurface = NULL;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *windowsurface = NULL;

int main(void)
{
	int quit = 0;

	//Load media
	if(!loadMedia())
		printf( "Failed to load media!\n" );

	if (init_instance() != 0) /*Initialize the SDL instance*/
		return (1);

	/**Set default surface*/
	gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
	while (!quit) /*DON'T Set up an infinite loop that you cant break*/
	{
		/*Get the window surface*/
		windowsurface = SDL_GetWindowSurface(window);
		/*Blit the image onto the window surface*/
		quit = poll_events();
		SDL_BlitSurface(gCurrentSurface, NULL, windowsurface, NULL);
		/*Update the surface*/
		SDL_UpdateWindowSurface(window);
		// SDL_RenderClear(renderer);
		//draw_stuff();
		//SDL_RenderPresent(renderer);
	}

	/*Hack to get window to stay up (Repurposed for c instead of c++)*/
    /*SDL_Event e; int quit = 0; while( quit == 0 ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = 1; } }*/

	close_SDL(); /*Function to release and free the SDL instance*/
    return (0);
}
