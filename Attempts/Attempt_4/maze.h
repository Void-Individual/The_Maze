#ifndef MAZE_H
#define MAZE_H

#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>

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
} SDL_Instance;

int init_instance(SDL_Instance *);
void draw_stuff(SDL_Instance *);
int poll_events(void);
void close_instance(SDL_Instance *);
int loadMedia(void);
SDL_Surface *loadSurface(char *path);

enum KeyPressSurfaces { /*Enumeration for key press surfaces*/
	    KEY_PRESS_SURFACE_DEFAULT,
	    KEY_PRESS_SURFACE_UP,
    	KEY_PRESS_SURFACE_DOWN,
    	KEY_PRESS_SURFACE_LEFT,
    	KEY_PRESS_SURFACE_RIGHT,
	    KEY_PRESS_SURFACE_TOTAL
	};

/*The images that correspond to a keypress*/
extern SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
extern SDL_Surface* gCurrentSurface;

#endif