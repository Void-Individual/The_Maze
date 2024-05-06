#include "maze.h"

/**
 * main - This is a function to run an sdl program
 * Return: 0
*/

int main(int argc, char *args[])
{
	// Start SSDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		// Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			// Apply the image
			SDL_BlitSurface(image_surface, NULL, screen_surface, NULL);
			// Update the surface
			SDL_UpdateWindowSurface(window);
			//Hack to get window to stay up (Repurposed for c instead of c++)
            SDL_Event e; int quit = 0; while( quit == 0 ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = 1; } }
		}
	}
	// Free resources and close SDL
	close();

	return (0);
}
