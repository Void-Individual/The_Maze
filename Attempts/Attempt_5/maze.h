#ifndef MAZE_H
#define MAZE_H

#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
 * struct SDL_Instance - A struct to hold data for an SDL instance
 * @window: The running window for the sdl instance
 * @renderer: The renderer for the instance
*/
//typedef struct SDL_Instance
//{
//    SDL_Window *window;
//    SDL_Renderer *renderer;
//    SDL_Surface *windowsurface;
//} SDL_Instance;

extern int SCREEN_HEIGHT;
extern int SCREEN_WIDTH;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Surface *windowsurface;
extern SDL_Texture* loadedTexture;

enum KeyPressSurfaces { /*Enumeration for key press surfaces*/
	    KEY_PRESS_SURFACE_DEFAULT,
	    KEY_PRESS_SURFACE_UP,
    	KEY_PRESS_SURFACE_DOWN,
    	KEY_PRESS_SURFACE_LEFT,
    	KEY_PRESS_SURFACE_RIGHT,
    	KEY_PRESS_SURFACE_p,
	    KEY_PRESS_SURFACE_TOTAL
	};
/*The images that correspond to a keypress*/
extern SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
extern SDL_Surface* gCurrentSurface;

//Button constants
extern int BUTTON_WIDTH;
extern int BUTTON_HEIGHT;
extern int TOTAL_BUTTONS;
enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

int init_instance(void);
void draw_stuff(void);
int poll_events(void);
void close_SDL(void);
int loadMedia(void);
SDL_Surface *loadSurface(char *path);
SDL_Texture *loadTexture(char *path);

#endif
