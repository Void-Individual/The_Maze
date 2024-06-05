#ifndef MAZE_H
#define MAZE_H
/*-----------------Standard Headers----------------------------------*/
#include "stdio.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
/*-------------------------------------------------------------------*/

/*-----------------Texture Array Declaration-------------------------*/
extern const int sky[];
extern const int start[];
extern const int win[];
extern const int win_tile[];
extern const int lose[];
extern const int open_door[];
extern const int closed_door[];
extern const int Textures5[];
extern const int red_brick[];
extern const int small_red_bricks[];
extern const int blue_tile_wall[];
extern const int metal_surface[];
/*-------------------------------------------------------------------*/

/*-----------------Map Variables-------------------------------------*/
#define mapX  8     /*map width*/
#define mapY  8      /*map height*/
#define mapS 64      /*map cube size*/
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640

/*-------------------------------------------------------------------*/

/*-----------------SDL2 Necessary Variables--------------------------*/

/*-------------------------------------------------------------------*/


/**
 * struct ButtonKeys - Struct to handle button commands
 * @w: Variable key for moving forward
 * @a: Variable key for turning left
 * @s: Variable key for moving backward
 * @d: Variable key for turning right
 * @o: Variable key to open doors
*/
typedef struct ButtonKeys
{
	int w, a, s, d, o;
} ButtonKeys; /*Struct for the movement controls and directions*/

/**
 * struct sprite - Struct to handle every sprite and their values
 * @type: To confirm the king of sprite
 * @state: To confirm if the sprite is active or not
 * @map: To confirm the texture to how on map7
 * @x: sprite position x
 * @y: sprite position y
 * @z: sprites height
*/

typedef struct sprite
{
	int type; /*Is it static, a key or an enemy?*/
	int state; /*Is it on or off*/
	int map; /*The texture to be shown*/
	int x, y, z; /*The position*/
} sprite;

typedef struct SDL_Instance
{
	int mapW[64];          /*walls*/
	int mapF[64];          /*floors*/
	int mapC[64];          /*ceiling*/

	SDL_Window *window;
	SDL_Renderer *renderer;

	float px, py, pdx, pdy, pa;
	float movementSpeed;
	float rotationSpeed;

	Uint32 frame1, frame2;
	float fps;

	int old_wall; /*Give the old wall an invalid value*/
	int door_open; /*Bool condition for toggling door*/
	int door; /*The door tile value*/

	int temp_door; /*If door has been opened, replace with this instead*/
	int gameState, timer;
	float fade;
	ButtonKeys keys;
	sprite *sp; /*This declares 4 sprites*/
	int depth[120];
} SDL_Instance;

void declareInstanceVariables(SDL_Instance *instance);

/*-----------------Custom SDL2 Functions-----------------------------*/
void SDL_SetRenderDrawColorF(SDL_Instance *instance, float r, float g, float b, float a);
void SDL_RenderDrawThickLine(SDL_Instance *instance, int x1, int y1, int x2, int y2, int thickness);
void SDL_RenderDrawLargerPoint(SDL_Instance *instance, int size, int x, int y);
void SDL_RenderDrawLargePoint(SDL_Instance *instance, int size, int x, int y);
/*-------------------------------------------------------------------*/

/*-----------------Functions To Draw Specific Parts------------------*/
void drawPlayer(SDL_Instance *instance);
void drawSprite(SDL_Instance *instance);
void drawScreen(SDL_Instance *instance, int v);
void drawSky(SDL_Instance *instance);
void drawMap2D(SDL_Instance *instance);
void drawRays(SDL_Instance *instance);

int *selectTextures(int hmt);
void setTextureColor(SDL_Instance *instance, int hmt, float shade, float tx, float ty, int wall,
	int floor, int ceiling, int mp);
/*-------------------------------------------------------------------*/

/*-----------------Function To Handle The Game States----------------*/
void init_gamestate(SDL_Instance *instance);
void init_game(SDL_Instance *instance);
void startGame(SDL_Instance *instance);
/*-------------------------------------------------------------------*/

/*-----------------Functions To Control Input------------------------*/
void ButtonDown(SDL_Instance *instance, SDL_KeyCode key);
void ButtonUp(SDL_Instance *instance, SDL_Keycode key);
int poll_events(SDL_Instance *instance);

/*-------------------------------------------------------------------*/

float FixAng(float a);
float degToRad(float a);

#endif
