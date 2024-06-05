#include "maze.h"

int *selectTextures(int hmt)
{
	/*Note that the hmt values are one behind the map values*/
	if (hmt == 0)
		return (red_brick);
	if (hmt == 1)
		return (small_red_bricks);
	if (hmt == 2)
		return (closed_door);
	if (hmt == 3)
		return (open_door);
	if (hmt == 4)
		return (win_tile);
	else
		return (metal_surface);
}

void setTextureColor(SDL_Instance *instance, int hmt, float shade, float tx, float ty, int wall, int floor, int ceiling, int mp)
{
	int pixel, red, green, blue, *Texture;
	if (floor)
	{
		pixel = ((((int)ty & 31)* 32) + ((int)tx & 31)) * 3 + mp * 3;
		red = Textures5[pixel+0] * 0.7;
		green = Textures5[pixel+1] * 0.7;
		blue = Textures5[pixel+2] * 0.7;
		SDL_SetRenderDrawColor(instance->renderer, red, green, blue, 255);
	}
	if (ceiling)
	{
		pixel = ((((int)ty & 31)* 32) + ((int)tx & 31)) * 3 + mp * 3;
		red = Textures5[pixel+0] * 0.7;
		green = Textures5[pixel+1] * 0.7;
		blue = Textures5[pixel+2] * 0.7;
		SDL_SetRenderDrawColor(instance->renderer, red, green, blue, 255);
	}
	if (wall)
	{
		Texture = selectTextures(hmt);
		pixel = ((int)ty*32 + (int)tx)*3;// + (hmt*32*32*3); /*Add hmt to retrieve the right texture per tile*/
		red = Texture[pixel+0]*shade;
		green = Texture[pixel+1]*shade;
		blue = Texture[pixel+2]*shade;
		SDL_SetRenderDrawColor(instance->renderer, red, green, blue, 225);

		if (hmt == 2 || hmt == 3)
			if ((red == 255 && green == 255 && blue == 255) ||
				(red == 255*shade && green == 255*shade && blue == 255*shade))
				SDL_SetRenderDrawColor(instance->renderer, 150, 75, 0, 255);

	}

}
