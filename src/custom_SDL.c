#include "maze.h"

/**
 * SDL_RenderDrawLargePoint - Function to draw a quad with multiplied values
 * into a large point.
 * @size: size of the quad
 * @x: starting x pos
 * @y: starting y pos
 * Return: Nothing
*/

void SDL_RenderDrawLargePoint(int size, int x, int y)
{
	SDL_Rect rect = {x - size/2, y - size/2, size, size};
	SDL_RenderFillRect(renderer, &rect);
}

/**
 * SDL_RenderDrawLargerPoint - Function to draw a quad with multiplied values
 * into a larger point.
 * @size: size of the quad
 * @x: starting x pos
 * @y: starting y pos
 * Return: Nothing
*/

void SDL_RenderDrawLargerPoint(int size, int x, int y)
{
	SDL_Rect rect = {x*size, y*size, size, size};
	SDL_RenderFillRect(renderer, &rect);
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

void SDL_RenderDrawThickLine(int x1, int y1, int x2, int y2, int thickness)
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
        SDL_RenderDrawLine(renderer, x1 + offsetX, y1 + offsetY, x2 + offsetX, y2 + offsetY);
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

void SDL_SetRenderDrawColorF(float r, float g, float b, float a)
{
    /*Ensure the values are clamped between 0.0 and 1.0*/
    if (r < 0.0f) r = 0.0f;
    if (r > 1.0f) r = 1.0f;
    if (g < 0.0f) g = 0.0f;
    if (g > 1.0f) g = 1.0f;
    if (b < 0.0f) b = 0.0f;
    if (b > 1.0f) b = 1.0f;
    if (a < 0.0f) a = 0.0f;
    if (a > 1.0f) a = 1.0f;

    /*Convert the float values to Uint8*/
    Uint8 r8 = (Uint8)(r * 255);
    Uint8 g8 = (Uint8)(g * 255);
    Uint8 b8 = (Uint8)(b * 255);
    Uint8 a8 = (Uint8)(a * 255);

    /*Set the render draw color*/
    SDL_SetRenderDrawColor(renderer, r8, g8, b8, a8);
}

void drawRectOutline(SDL_Renderer* renderer, SDL_Rect rect)
{
    /*Draw the top side*/
    SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y);

    /*Draw the left side*/
    SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x, rect.y + rect.h);

    /*Draw the bottom side*/
    SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h);

    /*Draw the right side*/
    SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
}

/**
 * loadSurface - FUnction to load image at a path
 * @path: Path to the image
 * Return: The loaded image
*/

//SDL_Surface *loadSurface(char *path)
//{
//	/*Load image at the specified path*/
//	SDL_Surface *loadedsurface = NULL;
//	SDL_Surface *optimizedSurface = NULL;

//	if (strcmp(path, "../image/welcome.png") == 0)
//		loadedsurface = IMG_Load(path);
//	else
//		loadedsurface = SDL_LoadBMP(path);

//	if (loadedsurface == NULL)
//		printf("Unable to load image %s\nError: %s\n", path, SDL_GetError());
//	else /*convert surface to screen format*/
//	{
//		optimizedSurface = SDL_ConvertSurface(loadedsurface, screen_surface->format, 0);
//		if (optimizedSurface == NULL)
//			printf("Unable to optimize image %s!\n Error: %s\n", path, SDL_GetError());
//		SDL_FreeSurface(loadedsurface); /*Get rid of old loaded surface*/
//	}

//	return (optimizedSurface);
//}

/**
 * loadTexture - Function to load an individual image as texture
 * @path: path to the image
 * Return: the texture
*/

SDL_Texture *loadTexture(char *path)
{
    SDL_Texture* newTexture = NULL; //The final texture
    SDL_Surface* loadedSurface = IMG_Load(path); //Load image at specified path

    if( loadedSurface == NULL )
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if( newTexture == NULL )
            printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}
