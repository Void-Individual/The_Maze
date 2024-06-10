#include "maze.h"

/**
 * init - Function to startup sdl and create a window
 * Return: 1 on success, else 0
*/

int init(void)
{
	// Initialization flag
	int success = 1;

	// Initialize sdl
	if (SDL_Init( SDL_INIT_VIDEO ) < 0)
	{
		printf("SDL couldn't be initialized!\nError: %s", SDL_GetError());
		success = 0;
	}
	else
	{
		// Create a window
		window = SDL_CreateWindow("My SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		// The sdl window shown variable is used to ensure that the window opens after creation

		if (window == NULL)
		{
			printf("Window could not be created!\nError: %s", SDL_GetError());
			success = 0;
		}
		else
		{
			// Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return (success);
}
