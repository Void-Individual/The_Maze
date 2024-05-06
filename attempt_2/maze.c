#include "maze.h"

int init_instance(SDL_Instance *instance)
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Unable to Initialize SDL: %s\n", SDL_GetError());
		return (1);
	}

	// Create a new window instance
	instance->window = SDL_CreateWindow("Void's SDL2 \\o/", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 640, 480, 0);
	if (instance->window == NULL)
	{
		fprintf(stderr, "Create window error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}

	// Create a new renderer instance linked to the window
	instance->renderer = SDL_CreateRenderer(instance->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (instance->renderer == NULL)
	{
		fprintf(stderr, "Create renderer error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}

	return (0);
}

void draw_stuff(SDL_Instance *instance, int x)
{
	if (x % 2 == 0)
	{
		SDL_SetRenderDrawColor(instance->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawLine(instance->renderer, 0, 0, x, x);
	}
	else
	{
		SDL_SetRenderDrawColor(instance->renderer, 100, 150, 200, 0xFF);
		SDL_RenderDrawLine(instance->renderer, 0, 0, x, x);
	}
}

int poll_events(void)
{
	SDL_Event event;
	SDL_KeyboardEvent key;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				return (1);
			case SDL_KEYDOWN:
				key = event.key;
				// If escape was pressed
				if (key.keysym.scancode == 0x29)
					return (1);
				break;
		}
	}

	return (0);
}

/**
 * main - Second attempt to learn to use sdl2
 * Return: 0
*/

int main(void)
{
    SDL_Instance *instance;
	int x = 0;

	// Allocate memory for instance
    instance = malloc(sizeof(SDL_Instance));
    if (instance == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for SDL_Instance\n");
        return (1);
    }

    if (init_instance(instance) != 0)
	{
		free(instance);
		return (1);
	}
	// DON'T Set up an infinite loop that you cant break

	while (1)
	{
		SDL_SetRenderDrawColor(instance->renderer, 0, 0, 0, 0);
		SDL_RenderClear(instance->renderer);
		if (poll_events() == 1)
			break;
		draw_stuff(instance, x);
		SDL_RenderPresent(instance->renderer);
		//sleep(1);
		x++;
	}
	// Hack to get window to stay up (Repurposed for c instead of c++)
    // SDL_Event e; int quit = 0; while( quit == 0 ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = 1; } }
	free(instance);
    return (0);
}
