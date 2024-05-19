#include "maze.h"

/**
 * init_instance - Function to initialize a SDL window
 * @instance: The SDL instance to work on
 * Return: 0 if successfull, else 1
*/

int init_instance(SDL_Instance *instance)
{
	/* Initialize SDL*/
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Unable to Initialize SDL: %s\n", SDL_GetError());
		return (1);
	}

	/*Create a new window instance*/
	instance->window = SDL_CreateWindow("Void's SDL2 \\o/",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
	if (instance->window == NULL)
	{
		fprintf(stderr, "Create window error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	else
	{
		/*Get the window surface*/
		instance->windowsurface = SDL_GetWindowSurface(instance->window);
		/*Fill the surface white*/
		SDL_FillRect(instance->windowsurface, NULL, SDL_MapRGB(
			instance->windowsurface->format, 0xFF, 0xFF, 0xFF ) );
		/*Update the surface*/
		SDL_UpdateWindowSurface(instance->window);
		/*Hack to get window to stay up (Repurposed for c instead of c++)*/
    	/*SDL_Event e; int quit = 0; while( quit == 0 ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = 1; } }*/
	}

	/*Create a new renderer instance linked to the window*/
	instance->renderer = SDL_CreateRenderer(instance->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (instance->renderer == NULL)
	{
		SDL_DestroyWindow(instance->window);
		fprintf(stderr, "Create renderer error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	return (0);
}
