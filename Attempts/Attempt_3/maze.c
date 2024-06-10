#include "maze.h"

/**
 * main - Second attempt to learn to use sdl2
 * Return: 0
*/

int main(void)
{
	SDL_Instance *instance; /* Create an SDL instance variable*/
	int x = 0;

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

	while (x < 10000) /*DON'T Set up an infinite loop that you cant break*/
	{
		SDL_SetRenderDrawColor(instance->renderer, 0, 0, 0, 0);
		SDL_RenderClear(instance->renderer);
		if (poll_events() == 1)
			break;
		draw_stuff(instance, x);
		SDL_RenderPresent(instance->renderer);
		x++;
	}

	/*Hack to get window to stay up (Repurposed for c instead of c++)*/
    /*SDL_Event e; int quit = 0; while( quit == 0 ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = 1; } }*/

	close_instance(instance); /*Function to release and free the SDL instance*/
    return (0);
}
