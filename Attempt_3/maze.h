#ifndef MAZE_H
#define MAZE_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

/**
 * struct SDL_Instance - A struct to hold data for an SDL instance
 * @window: The running window for the sdl instance
 * @renderer: The renderer for the instance
*/
typedef struct SDL_Instance
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *windowsurface;
    SDL_Surface *imagesurface;
} SDL_Instance;

int init_instance(SDL_Instance *);
void draw_stuff(SDL_Instance *, int x);
int poll_events(void);
void close_instance(SDL_Instance *);
int loadMedia(SDL_Instance *);

#endif
