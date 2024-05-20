#include "maze.h"

/**
 * draw_stuff - Function to draw lines on the window
 * @instance: SDL instance to work with
 * Return: Empty
*/

void draw_stuff(SDL_Instance *instance)
{
	SDL_SetRenderDrawColor(instance->renderer, 0xFF, 0, 0xFF, 0xFF);
	SDL_RenderDrawLine(instance->renderer, 10, 10, 100, 100);
}
