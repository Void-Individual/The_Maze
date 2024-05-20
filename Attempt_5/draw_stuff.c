#include "maze.h"

/**
 * draw_stuff - Function to draw lines on the window
 * Return: Empty
*/

void draw_stuff(void)
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0xFF, 0xFF);
	SDL_RenderDrawLine(renderer, 10, 10, 100, 100);
}
