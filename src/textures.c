#include "maze.h"

/**
 * drawSprite - Function to draw sprites
 * @type: variable to request for specific sprite
 * Return: Nothing
*/

void *spriteTexture(int type)
{
	if (type == 1)
		return (key);
	if (type == 2)
		return (babe);
	if (type == 3)
		return (enemy);
	if (type == 4)
		return (enemy2);
	if (type == 5)
		return (toilet_guy);
}

/**
 * selectTextures - Function to select specific wall texture
 * @hmt: surface texture value
 * Return: pointer to an array of textures
 */

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

/**
 * setTextureColor: Function to set the rendere colour to rgb
 * @hmt: The surface texture value
 * @shade: the intensity of light to be drawn on the surface
 * @tx: the x axis of the surface to be textured
 * @ty: the y axis of the surface to be textured
 * @wall: if the surface is a wall, this var should be 1, else 0
 * @floor: if the surface is a floor, this var should be 1, else 0
 * @ceiling: if the surface is a ceiling, this var should be 1, else 0
 * @mp: this variable is only needed if either ceiling or floor is 1
 * Return: Nothing
*/

void setTextureColor(int hmt, float shade, float tx, float ty, int wall, int floor, int ceiling, int mp)
{
	int pixel, red, green, blue, *Texture;
	if (floor)
	{
		pixel = ((((int)ty & 31)* 32) + ((int)tx & 31)) * 3 + mp * 3;
		red = floor_textures[pixel+0] * 0.7;
		green = floor_textures[pixel+1] * 0.7;
		blue = floor_textures[pixel+2] * 0.7;
		SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
	}
	if (ceiling)
	{
		pixel = ((((int)ty & 31)* 32) + ((int)tx & 31)) * 3 + mp * 3;
		red = ceiling_textures[pixel+0] * 0.7;
		green = ceiling_textures[pixel+1] * 0.7;
		blue = ceiling_textures[pixel+2] * 0.7;
		SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
	}
	if (wall)
	{
		Texture = selectTextures(hmt);
		pixel = ((int)ty*32 + (int)tx)*3;// + (hmt*32*32*3); /*Add hmt to retrieve the right texture per tile*/
		red = Texture[pixel+0]*shade;
		green = Texture[pixel+1]*shade;
		blue = Texture[pixel+2]*shade;
		SDL_SetRenderDrawColor(renderer, red, green, blue, 225);

		if (hmt == 2 || hmt == 3)
			if ((red == 255 && green == 255 && blue == 255) ||
				(red == 255*shade && green == 255*shade && blue == 255*shade))
				SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255);

	}
}
