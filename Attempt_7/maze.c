#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.1415926535
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533 /*One degree in radians*/

int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 512;

float px = 300, py = 300; /*Player position*/
float pdx, pdy, pa; /*delta x, y and angle of the player*/
float p_size = 10; /*Player size*/
float p_px, p_py; /*The center point ofthe player for the rays*/
Uint64 frame1 = 0, frame2 = 0;
float fps;
float movementSpeed = 0.1; /*Original movement speed*/

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *screen_surface = NULL;

typedef struct ButtonKeys {
	int w, a, s, d;
}ButtonKeys; /*Struct for the movement controls and directions*/
ButtonKeys keys;

int mapX = 8, mapY = 8, tileSize = 64;
int map[] = {
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 1, 1, 1, 1,
	1, 1, 0, 0, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};

int Check_Textures[] = { /*Checkerboard 32x32 Texture*/
	0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 1, 1, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 1, 1, 1, 1, 1, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 1, 1, 1, 1, 1, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 1, 1, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,

	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,

	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 0,
};

int Brick_Textures[] = {/*Brick 32x32 Texture*/
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,		0, 1, 1, 1, 1, 1, 1, 1,		1, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0
};

int Door_Texture[] = { /*Door 32x32 Texture*/
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 1,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		1, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 1,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		1, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 1,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		1, 0, 0, 1, 0, 0, 0, 0,

	0, 0, 0, 0, 1, 0, 0, 1,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		1, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 1,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		1, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 1,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		1, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 1,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		1, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 1,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		1, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 1,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		1, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 1,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		1, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 1,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		1, 1, 1, 1, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 1, 0, 1,		1, 0, 1, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 1, 1, 1, 1, 0, 1,		1, 0, 1, 1, 1, 1, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 0,

	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,		1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 0
};

int Window_Texture[] = { /*Window 32x32 Texture*/
	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,

	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1,


	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,

	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1, 	1, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1
};

float fixAng(float a)
{
	if (a > PI)	a -= 2*PI;
	if (a < 0)		a += 2*PI;
	return (a);
}

void drawMap2D(void)
{
	int x, y, xo, yo;

	for (y = 0; y < mapY; y++)
		for (x = 0; x < mapX; x++)
		{
			if (map[y * mapX + x] == 1)
				SDL_SetRenderDrawColor(renderer, 220, 220, 150, 255); /*White, later customized*/
			else
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); /*Black, later customized*/

			xo = x * tileSize;
			yo = y * tileSize;

			SDL_Rect rect = {xo - 1, yo - 1, tileSize - 1, tileSize - 1};
            SDL_RenderFillRect(renderer, &rect);
		}
}

float dist(float ax, float ay, float bx, float by, float ang)
{
	(void) ang;
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}


void drawRays2D(void)
{
	int r, mx, my, mp, dof;
	float rx, ry, ra, xo, yo, disT;

	ra = pa - DR * 30; /*Ray angle is the same as the player angle*/
	if (ra < 0) ra += 2 * PI;
	if (ra > 2 * PI) ra -= 2 * PI;

	for (r = 0; r < 60; r++)
	{
		/*We will check for the ray hitting horizontal lines*/
		dof = 0; /*Depth of field*/
		float disH = 100000, hx = px, hy = py;
		float aTan = -1 / tan(ra);
		if (ra > PI) /*The ray is looking up*/
		{
			ry = (((int)py >> 6) << 6) - 0.0001;
			rx = (py - ry) * aTan + px;
			yo = -64; xo = -yo * aTan;
		}
		if (ra < PI) /*The ray is looking down*/
		{
			ry = (((int)py >> 6) << 6) + 64;
			rx = (py - ry) * aTan + px;
			yo = 64; xo = -yo * aTan;
		}
		if (ra == 0 || ra == PI) /*The ray is looking left or right*/
			{rx = px; ry = py; dof = 8;}
		while (dof < 8)
		{
			mx = (int) (rx) >> 6;
			my = (int) (ry) >> 6;
			mp = my * mapX + mx;
			if (mp >= 0 && mp < mapX*mapY && map[mp] == 1) /*The map point hit a wall*/
			{
				dof = 8; /*Shut the loop down*/
				hx = rx;
				hy = ry;
				disH = dist(px, py, hx, hy, ra);
			}
			else /*If the ray doesn't hit a wall*/
				{rx += xo; ry += yo, dof += 1;} /*Find the next horizontal line*/
		}

		/*We will check for the ray hitting vertical lines*/
		dof = 0;
		float disV = 100000, vx = px, vy = py;
		float nTan = -tan(ra);
		if (ra > P2 && ra < P3) /*The ray is looking left*/
		{
			rx = (((int)px >> 6) << 6) - 0.0001;
			ry = (px - rx) * nTan + py;
			xo = -64; yo = -xo * nTan;
		}
		if (ra < P2 || ra > P3) /*The ray is looking right*/
		{
			rx = (((int)px >> 6) << 6) + 64;
			ry = (px - rx) * nTan + py;
			xo = 64; yo = -xo * nTan;
		}
		if (ra == 0 || ra == PI) /*The ray is looking straight up or down*/
			{rx = px; ry = py; dof = 8;}
		while (dof < 8)
		{
			mx = (int) (rx) >> 6;
			my = (int) (ry) >> 6;
			mp = my * mapX + mx;
			if (mp >= 0 && mp < mapX*mapY && map[mp] == 1) /*The map point hit a wall*/
			{
				dof = 8; /*Shut the loop down*/
				vx = rx;
				vy = ry;
				disV = dist(px, py, vx, vy, ra);
			}
			else /*If the ray doesn't hit a wall*/
				{rx += xo; ry += yo, dof += 1;} /*Find the next vertical line*/
		}

		if (disV < disH) /*Vertical wall was hit*/
		{
			rx = vx;
			ry = vy;
			disT = disV;
			SDL_SetRenderDrawColor(renderer, 0, 220, 0, 255);
		}
		if (disH < disV) /*Horizontal wall was hit*/
		{
			rx = hx;
			ry = hy;
			disT = disH;
			SDL_SetRenderDrawColor(renderer, 0, 170, 0, 255);
		}

		int rt = 8; /*Set the desired ray thickness*/
		for (int i = -rt/2; i <= rt/2; i++)
		{
			SDL_RenderDrawLine(renderer, px + i, py, rx + i, ry);
			SDL_RenderDrawLine(renderer, px, py + i, rx, ry + i);
		}

		/*Draw the 3D walls*/
		float ca = pa - ra;
		if (ca < 0) ca += 2 * PI;
		if (ca > 2 * PI) ca -= 2 * PI;
		disT = disT*cos(ca); /*This is to fix the fisheye effect*/
		float lineH = (tileSize*320) / disT;
		if (lineH > 320) lineH = 320; /*Line/Wall height*/
		float lineO = 160 - lineH / 2; /*Line offset*/

		for (int y = 0; y < lineH; y++)
		{
			for (int i = -rt/2; i <= rt/2; i++) /*Loop to draw the walls*/
			{
				SDL_RenderDrawPoint(renderer, r * 8+530 + i, lineO + y);
				SDL_RenderDrawPoint(renderer, r * 8+530, lineO + i + y);
			}
		}

		ra += DR;
		ra = fixAng(ra);
	}
}

void drawPlayer(void)
{
	/*To prevent player from walking out of the screen*/
	if (px < 1)		px = 1;
	if (py < 1)		py = 1;
	if (px > SCREEN_WIDTH)		px = SCREEN_WIDTH - 1;
	if (py > SCREEN_HEIGHT)		py = SCREEN_HEIGHT - 1;

	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_Rect rect = { px - p_size / 2, py - p_size / 2, p_size, p_size};
    SDL_RenderFillRect(renderer, &rect);

	//SDL_RenderDrawLine(renderer, px, py, px+pdx*5, py+pdy*5);
}

void ButtonDown(SDL_KeyCode key)
{
	if (key == SDLK_UP || key == SDLK_w) /*Move forward*/
		keys.w = 1;
	if (key == SDLK_DOWN || key == SDLK_s) /*Move backwards*/
		keys.s = 1;
	if (key == SDLK_LEFT || key == SDLK_a) /*Turn left*/
		keys.a = 1;
	if (key == SDLK_RIGHT || key == SDLK_d) /*Turn right*/
		keys.d = 1;
}

void ButtonUp(SDL_Keycode key)
{
	if (key == SDLK_UP || key == SDLK_w) /*Move forward*/
		keys.w = 0;
	if (key == SDLK_DOWN || key == SDLK_s) /*Move backwards*/
		keys.s = 0;
	if (key == SDLK_LEFT || key == SDLK_a) /*Turn left*/
		keys.a = 0;
	if (key == SDLK_RIGHT || key == SDLK_d) /*Turn right*/
		keys.d = 0;
}

int poll_events(void)
{
	SDL_Event event; /*Holds events from SDL event queue*/

	while (SDL_PollEvent(&event)) /*Loop to check through any pending events*/
	{
		if (event.type == SDL_QUIT)
			return (1);
		if (event.type == SDL_KEYDOWN)
			ButtonDown(event.key.keysym.sym);
		if (event.type == SDL_KEYUP)
			ButtonUp(event.key.keysym.sym);
	}
	return (0);
}

void display(void)
{
	/*Frames per second, implement this later*/
	frame2 = SDL_GetPerformanceCounter();
	float elapsedTime = (frame2 - frame1) / (float)SDL_GetPerformanceFrequency();
	fps = 1.0f / elapsedTime;
	frame1 = frame2;

	/*Buttons*/
	float movementSpeed = 25.0f * elapsedTime;
	if (keys.a) /*Turn left*/
	{
		pa -= 0.1f;
		pa = fixAng(pa);
		pdx = cos(pa) * movementSpeed;
		pdy = sin(pa) * movementSpeed;
	}
	if (keys.d) /*Turn right*/
	{
		pa += 0.1;
		pa = fixAng(pa);
		pdx = cos(pa) * movementSpeed;
		pdy = sin(pa) * movementSpeed;
	}

	/*Variables required to stop player from moving forward and backward through walls*/
	int xo = 0; if (pdx < 0) xo = -20; else xo = 20;
	int yo = 0; if (pdy < 0) yo = -20; else yo = 20;
	int ipx = px/64.0, ipx_add_xo = (px+xo)/64.0, ipx_sub_xo = (px-xo)/64.0;
	int ipy = py/64.0, ipy_add_yo = (py+yo)/64.0, ipy_sub_yo = (py-yo)/64.0;

	if (keys.w) /*Move forward*/
	{
		if (map[ipy * mapX + ipx_add_xo] == 0)
			px += pdx * 5;
		if (map[ipy_add_yo * mapX + ipx] == 0)
			py += pdy * 5; /*Extra movement values*/
	}
	if (keys.s) /*Move backwards*/
	{
		if (map[ipy * mapX + ipx_sub_xo] == 0)
			px -= pdx * 5; /*Extra movement values*/
		if (map[ipy_sub_yo * mapX + ipx] == 0)
			py -= pdy * 5; /*Extra movement values*/
	}
	drawMap2D();
	drawRays2D();
	drawPlayer();
}

int init(void)
{
	/* Initialize SDL*/
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Unable to Initialize SDL: %s\n", SDL_GetError());
		return (1);
	}

	/*Create a new window*/
	window = SDL_CreateWindow("Void's SDL2 \\o/",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (window == NULL)
	{
		fprintf(stderr, "Create window error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	//screen_surface = SDL_GetWindowSurface(window);

	/*Create a new renderer instance linked to the window*/
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		SDL_DestroyWindow(window);
		fprintf(stderr, "Create renderer error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}

	/*Declare the players starting point*/
	px = 300; py = 300;
	/*Declare the players starting angles*/
	pa = 0;	pdx =  cos(pa) * movementSpeed; pdy =  sin(pa) * movementSpeed;

	return (0);
}

int main(void)
{
	if (init() != 0)
	{
		printf("Failed to init\n");
		return (1);
	}
	while (1)
	{
		//Initialize renderer color
	    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
		SDL_RenderClear(renderer);
		display();
		if (poll_events() == 1)
			break;
		SDL_RenderPresent(renderer); // Updtae the screen

	}
	/*Cleanup*/
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return (0);
}
