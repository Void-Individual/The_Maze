#include "maze.h"

/**
 * drawMap2D - Function to draw the 2D map of the game
 * Return: Nothing
*/

void drawMap2D(SDL_Instance *instance)
{
	int x, y, xo, yo;

	for (y = 0; y < mapY; y++)
		for (x = 0; x < mapX; x++)
		{
			xo = x * mapS;
			yo = y * mapS;
			if (instance->mapW[y * mapX + x] > 0)
			{
				int a, b;
				int ty = instance->mapW[y * mapX + x] * 32 * 32 * 3;

				for (a = 1; a < 63; a += 2)
					for (b = 1; b < 63; b += 2)
					{

						int pixel = ((a / 2) % 32 * 32 + (b / 2) % 32) * 3 + ty;
						int red = Textures5[pixel + 0];
						int green = Textures5[pixel + 1];
						int blue = Textures5[pixel + 2];

						SDL_SetRenderDrawColor(instance->renderer, red, green, blue, 255);
						SDL_Rect rect = {xo + b, yo + a, 2, 2};

						SDL_RenderFillRect(instance->renderer, &rect);
					}
			}
			else
			{
				SDL_SetRenderDrawColor(instance->renderer, 165, 42, 42, 255); /*Brown color*/
				SDL_Rect rect = {xo, yo, mapS - 1, mapS - 1};

				SDL_RenderFillRect(instance->renderer, &rect);
			}
		}
}

/**
 * drawPlayer - Function to draw the player on the 2D map
 * Return: Nothing
*/

void drawPlayer(SDL_Instance *instance)
{
	/*To prevent player from walking out of the screen*/
	if (instance->px < 1)
		instance->px = 1;
	if (instance->py < 1)
		instance->py = 1;
	if (instance->px > SCREEN_WIDTH)
		instance->px = SCREEN_WIDTH - 1;
	if (instance->py > SCREEN_HEIGHT)
		instance->py = SCREEN_HEIGHT - 1;

	SDL_SetRenderDrawColor(instance->renderer, 255, 0, 255, 255);
	SDL_RenderDrawLargePoint(instance, 8, instance->px, instance->py);

	/*Small directional pointer*/
	/*SDL_RenderDrawLine(instance->renderer, instance->px, instance->py, instance->px+pdx*10, instance->py+pdy*10);*/
}

/**
 * drawSky - Function to draw the sky
 * Return: Nothing
*/

void drawSky(SDL_Instance *instance)
{
	int x, y;

	/*Because we never see below the horizon, we only need the top half*/
	for (y = 0; y < 40; y++)
		for (x = 0; x < 120; x++)
		{
			/*To allow the sky to rotate as you turn, use player angle instead*/
			int xo = (int)instance->pa * 2 - x;

			if (xo < 0)
				xo += 120;
			xo = xo % 120;
			int pixel = (y * 120 + xo) * 3;
			int red = sky[pixel + 0];
			int green = sky[pixel + 1];
			int blue = sky[pixel + 2];

			SDL_SetRenderDrawColor(instance->renderer, red, green, blue, 255);
			SDL_RenderDrawLargePoint(instance, 8, x * 8, y * 8);
		}
}

/**
 * drawScreen - Function to draw full screen pages
 * @v: Version of the screen to draw
 * Return: Nothing
*/

void drawScreen(SDL_Instance *instance, int v)
{
	int x, y;
	int *T;

	if (v == 1)
		T = start;
	if (v == 2)
		T = win;
	if (v == 3)
		T = lose;
	for (y = 0; y < 80; y++)
		for (x = 0; x < 120; x++)
		{
			int pixel = (y * 120 + x) * 3;
			int red = T[pixel + 0] * instance->fade;
			int green = T[pixel + 1] * instance->fade;
			int blue = T[pixel + 2] * instance->fade;

			SDL_SetRenderDrawColor(instance->renderer, red, green, blue, 255);
			SDL_RenderDrawLargePoint(instance, 8, x * 8, y * 8);
		}
	/*Use instance->fade value to slowly the intensity of display*/
	if (instance->fade < 1)
		instance->fade += 0.005 * (3000.0 / instance->fps);
	if (instance->fade > 1)
		instance->fade = 1;
}

/**
 * drawSprite - Function to draw sprites
 * Return: Nothing
*/

void drawSprite(SDL_Instance *instance)
{
	/*This is done using a 3D projection matrix*/

	float sx = instance->sp[0].x - instance->px; /*Temp float variables*/
	float sy = instance->sp[0].y - instance->py;
	float sz = instance->sp[0].z;
	/*Rotate the sprite around its origin*/
	float CS = cos(degToRad(instance->pa)); /*sine and cosine rotation matrix*/
	float SH = sin(degToRad(instance->pa));
	float a = sy * CS + sx * SH;
	float b = sx * CS - sx * SH;

	sx = a;
	sy = b;
	/*Convert it to screen x, y*/
	sx = (sx * 108.0 / sy) + (120 / 2);
	sy = (sz * 108.0 / sy) + (80 / 2);

	/*Draw a point*/
	if (sx > 0 && sx < 120 && b < instance->depth[(int)sx])
	{
		/*Draw this sprite if its onscreen and if its depth is closer than the wall*/
		SDL_SetRenderDrawColor(instance->renderer, 215, 255, 0, 255);
		SDL_RenderDrawLargePoint(instance, 8, sx * 8, sy * 8);
	}
}
