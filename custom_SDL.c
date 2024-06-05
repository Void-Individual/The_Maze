#include "maze.h"

/**
 * SDL_RenderDrawLargePoint - Function to draw a quad with multiplied values
 * into a large point.
 * @size: size of the quad
 * @x: starting x pos
 * @y: starting y pos
 * Return: Nothing
*/

void SDL_RenderDrawLargePoint(SDL_Instance *instance, int size, int x, int y)
{
	SDL_Rect rect = {x - size / 2, y - size / 2, size, size};

	SDL_RenderFillRect(instance->renderer, &rect);
}

/**
 * SDL_RenderDrawLargerPoint - Function to draw a quad with multiplied values
 * into a larger point.
 * @size: size of the quad
 * @x: starting x pos
 * @y: starting y pos
 * Return: Nothing
*/

void SDL_RenderDrawLargerPoint(SDL_Instance *instance, int size, int x, int y)
{
	SDL_Rect rect = {x * size, y * size, size, size};

	SDL_RenderFillRect(instance->renderer, &rect);
}

/**
 * SDL_RenderDrawThickLine - function to draw a thick line using rect
 * @x1: Starting x pos
 * @y1: starting y pos
 * @x2: ending x pos
 * @y2: ending y pos
 * @thickness: thickness of the line
 * Return: Nothing
*/

void SDL_RenderDrawThickLine(SDL_Instance *instance, int x1, int y1, int x2, int y2, int thickness)
{
    /*Calculate the direction of the line*/
	float dx = x2 - x1;
	float dy = y2 - y1;
	float length = sqrt(dx * dx + dy * dy);

	dx /= length;
	dy /= length;

	/*Calculate the perpendicular direction*/
	float px = -dy;
	float py = dx;

	/*Draw multiple lines to create a thick line*/
	for (int i = -thickness / 2; i <= thickness / 2; i++)
	{
		int offsetX = px * i;
		int offsetY = py * i;

		SDL_RenderDrawLine(instance->renderer, x1 + offsetX, y1 + offsetY, x2 + offsetX,
			y2 + offsetY);
	}
}

/**
 * SDL_SetRenderDrawColorF - Function to select colour palette using float
 * values between 0 and 1 instead of int values 0 - 255
 * @r: float red value
 * @g: float green value
 * @b: float blue value
 * @a: float alpha value
 * Return: Nothing
*/

void SDL_SetRenderDrawColorF(SDL_Instance *instance, float r, float g, float b, float a)
{
    /*Ensure the values are clamped between 0.0 and 1.0*/
	if (r < 0.0f)
		r = 0.0f;
	if (r > 1.0f)
		r = 1.0f;
	if (g < 0.0f)
		g = 0.0f;
	if (g > 1.0f)
		g = 1.0f;
	if (b < 0.0f)
		b = 0.0f;
	if (b > 1.0f)
		b = 1.0f;
	if (a < 0.0f)
		a = 0.0f;
	if (a > 1.0f)
		a = 1.0f;

    /*Convert the float values to Uint8*/
	Uint8 r8 = (Uint8)(r * 255);
	Uint8 g8 = (Uint8)(g * 255);
	Uint8 b8 = (Uint8)(b * 255);
	Uint8 a8 = (Uint8)(a * 255);

	/*Set the render draw color*/
	SDL_SetRenderDrawColor(instance->renderer, r8, g8, b8, a8);
}
