#include "maze.h"

/**
 * draw_stuff - Function to draw lines on the window
 * @instance: SDL instance to work with
 * @x: Loop count
 * Return: Empty
*/

void draw_stuff(SDL_Instance *instance, int x)
{
	if (x % 2 == 0) /*Draw a white line*/
	{
		SDL_SetRenderDrawColor(instance->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawLine(instance->renderer, 0, 0, x, x);
	}
	else /*Draw a coloured line*/
	{
		SDL_SetRenderDrawColor(instance->renderer, 100, 150, 200, 0xFF);
		SDL_RenderDrawLine(instance->renderer, 0, 0, x, x);
	}
}
