#ifndef MAZE_H
#define MAZE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
/*-----------------Map constants-------------------------------------*/
#define mapX  8     /*map width*/
#define mapY  8      /*map height*/
#define mapS 64      /*map cube size*/
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640
/*-------------------------------------------------------------------*/

/*-----------------Texture Array Declaration-------------------------*/
extern int sky[];
extern int start[];
extern int win[];
extern int win_tile[];
extern int lose[];
extern int open_door[];
extern int closed_door[];
extern int Textures5[];
extern int red_brick[];
extern int small_red_bricks[];
extern int blue_tile_wall[];
extern int metal_surface[];
extern int key[];
extern int babe[];
extern int floor_textures[];
extern int map_textures[];
extern int ceiling_textures[];
extern int enemy[];
extern int enemy2[];
extern int toilet_guy[];
/*-------------------------------------------------------------------*/

/*-----------------Map Variables-------------------------------------*/
extern int mapW[];          /*walls*/
extern int mapF[];          /*floors*/
extern int mapC[];          /*ceiling*/
/*-----------------SDL2 Necessary Variables--------------------------*/
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Surface *screen_surface;

extern float px, py, pdx, pdy, pa;
extern float movementSpeed;
extern float rotationSpeed;

extern Uint32 frame1,frame2;
extern float fps;

extern int old_wall; /*Give the old wall an invalid value*/
extern int door_open; /*Bool condition for toggling door*/
extern int door; /*The door tile value*/
extern int temp_door; /*If the door has been opened, replace with this instead*/

extern int gameState, timer;
extern float fade;
extern int depth[120];

/**
 * struct ButtonKeys - Struct to handle button commands
 * @w: Variable key for moving forward
 * @a: Variable key for turning left
 * @s: Variable key for moving backward
 * @d: Variable key for turning right
 * @o: Variable key to open doors
 * @m: Variable to show and hide map
*/

typedef struct ButtonKeys {
	int w, a, s, d, o, m;
}ButtonKeys; /*Struct for the movement controls and directions*/
extern ButtonKeys keys;

/**
 * struct sprite - Struct to handle every sprite and their values
 * @type: To confirm the king of sprite
 * @state: To confirm if the sprite is active or not
 * @map: To confirm the texture to how on map7
 * @x: sprite position x
 * @y: sprite position y
 * @z: sprites height
*/

typedef struct
{
	int type; /*Is it static, a key or an enemy?*/
	int state; /*Is it on or off*/
	int map; /*The texture to be shown*/
	float x, y, z; /*The position*/
} sprite;
extern sprite sp[4]; /*This declares 4 sprites*/


/*-----------------Custom SDL2 Functions-----------------------------*/
void SDL_SetRenderDrawColorF(float r, float g,
	float b, float a);
void SDL_RenderDrawThickLine(int x1, int y1, int x2,
	int y2, int thickness);
void SDL_RenderDrawLargerPoint(int size, int x, int y);
void SDL_RenderDrawLargePoint(int size, int x, int y);
/*-------------------------------------------------------------------*/

/*-----------------Functions To Draw Specific Parts------------------*/
void drawPlayer(void);
void *spriteTexture(int type);
void drawSprite(void);
void drawScreen(int v);
void drawSky(void);
void drawMap2D(void);
void drawRays(void);
void display(void);
void displayWelcomeImage(void);

SDL_Texture *loadTexture(char *path);
int *selectTextures(int hmt);
void setTextureColor(int hmt, float shade, float tx,
	float ty, int wall, int floor, int ceiling, int mp);
/*-------------------------------------------------------------------*/

/*-----------------Function To Handle The Game States----------------*/
void init_gamestate(void);
void init_game(void);
void startGame(void);
/*-------------------------------------------------------------------*/

/*-----------------Functions To Control Input------------------------*/
void ButtonDown(SDL_KeyCode key);
void ButtonUp(SDL_Keycode key);
int poll_events(void);
/*-------------------------------------------------------------------*/

float FixAng(float a);
float degToRad(float a);

#endif
