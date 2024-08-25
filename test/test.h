#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>

#define mapX  8     /*map width*/
#define mapY  8      /*map height*/
#define mapS 64      /*map cube size*/
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640

typedef struct SDL_instance
{
	SDL_Window *window;
	SDL_Renderer * renderer;

}SDL_instance;

#endif
