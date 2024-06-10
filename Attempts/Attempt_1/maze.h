#ifndef MAZE_H
#define MAZE_H

// Include the main sdl2 library
#include <SDL2/SDL.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Initialize window
SDL_Window *window = NULL;
// Initialize the surface
SDL_Surface *screen_surface = NULL;
// The image will load and show on the screen
SDL_Surface *image_surface = NULL;

int init(void);
int loadMedia(void);
void close(void);
#endif
