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
	int w, a, s, d, o;
}ButtonKeys; /*Struct for the movement controls and directions*/
ButtonKeys keys;

float fixAng(float a)
{
	if (a > PI)	a -= 2*PI;
	if (a < 0)		a += 2*PI;
	return (a);
}

float dist(float ax, float ay, float bx, float by, float ang)
{
	(void) ang;
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

/*--------------------MAP----------------------------------------------------*/
int mapX = 8, mapY = 8, tileSize = 64;
int mapW[]= {          //walls
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 1, 0, 0, 0, 1,
	1, 0, 0, 2, 0, 0, 0, 1,
	1, 1, 2, 1, 0, 0, 0, 3,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};

int mapF[]= {          //floors
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 2, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 2, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};

int mapC[]= {          //ceiling
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0,
	0, 1, 3, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};

int All_Textures[] = {
	/*Checkerboard 32x32 Texture*/
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

	/*Brick 32x32 Texture*/
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
	0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,		0, 0, 0, 0, 0, 0, 0, 0,

	/*Door 32x32 Texture*/
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
	0, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 0,

	/*Window 32x32 Texture*/
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
	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1, 	1, 1, 1, 1, 1, 1, 1, 1,
};

void drawMap2D(void)
{
	int x, y, xo, yo;

	for (y = 0; y < mapY; y++)
		for (x = 0; x < mapX; x++)
		{
			if (mapW[y * mapX + x] > 0)
				SDL_SetRenderDrawColor(renderer, 220, 220, 150, 255); /*White, later customized*/
			else
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); /*Black, later customized*/

			xo = x * tileSize;
			yo = y * tileSize;

			SDL_Rect rect = {xo - 1, yo - 1, tileSize - 1, tileSize - 1};
            SDL_RenderFillRect(renderer, &rect);
		}
}
/*---------------------------------------------------------------------------*/

/*--------------------------PLAYER-------------------------------------------*/
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
/*---------------------------------------------------------------------------*/

/*--------------------------2D AND 3D RAYS-----------------------------------*/
//5548124145 ALX Costain hub wifi password

void DrawThickLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int thickness)
{
	float angle = atan2(y2 - y1, x2 - x1);
	float halfthickness = thickness / 2.0f;

	/*Corners of the thick line*/
	int xo = (int)(halfthickness * sin(angle));
	int yo = (int)(halfthickness * cos(angle));

	SDL_Point points[4] = {
		{x1 - xo, y1 + yo},
		{x1 + xo, y1 - yo},
		{x2 + xo, y2 - yo},
		{x2 - xo, y2 + yo}
	};

	/*Draw the filled rectangle as a thick line*/
	SDL_RenderDrawLines(renderer, points, 4);
	/*CLosing the rectangle*/
	SDL_RenderDrawLine(renderer, points[0].x, points[0].y, points[3].x, points[3].y);
}

void SDL_SetRenderDrawColorF(SDL_Renderer *renderer, float r, float g, float b, float a) {
    // Ensure the values are clamped between 0.0 and 1.0
    if (r < 0.0f) r = 0.0f;
    if (r > 1.0f) r = 1.0f;
    if (g < 0.0f) g = 0.0f;
    if (g > 1.0f) g = 1.0f;
    if (b < 0.0f) b = 0.0f;
    if (b > 1.0f) b = 1.0f;
    if (a < 0.0f) a = 0.0f;
    if (a > 1.0f) a = 1.0f;

    // Convert the float values to Uint8
    Uint8 r8 = (Uint8)(r * 255);
    Uint8 g8 = (Uint8)(g * 255);
    Uint8 b8 = (Uint8)(b * 255);
    Uint8 a8 = (Uint8)(a * 255);

    // Set the render draw color
    SDL_SetRenderDrawColor(renderer, r8, g8, b8, a8);
}

//void ApplyShading(float c, int hmt, float shade)
//{
//	int color = 255;
//	//printf("color: %d\n", color);
//	if (c < 1)
//		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//	else
//	{
//		if (shade)
//		{
//			color = color/1.5;
//			if (hmt == 0) /*Make check texture red*/
//				SDL_SetRenderDrawColor(renderer, color, color/2, color/2, 255);
//			if (hmt == 1) /*Make brick yellow*/
//				SDL_SetRenderDrawColor(renderer, color, color, color/2, 255);
//			if (hmt == 2) /*Make doors blue*/
//				SDL_SetRenderDrawColor(renderer, color/2, color/2, color, 255);
//			if (hmt == 3) /*Make windows green*/
//				SDL_SetRenderDrawColor(renderer, color, color/2, color, 255);
//		}
//		else
//		{
//			if (hmt == 0) /*Make check texture red*/
//				SDL_SetRenderDrawColor(renderer, color, color/2, color/2, 255);
//			if (hmt == 1) /*Make brick yellow*/
//				SDL_SetRenderDrawColor(renderer, color, color, color/2, 255);
//			if (hmt == 2) /*Make doors blue*/
//				SDL_SetRenderDrawColor(renderer, color/2, color/2, color, 255);
//			if (hmt == 3) /*Make windows green*/
//				SDL_SetRenderDrawColor(renderer, color, color/2, color, 255);
//		}
//	}
//}

void drawRays2D(void)
{
	int r, mx, my, mp, dof;
	float rx, ry, ra, xo, yo, disT;
	int vmt = 0, hmt = 0; /*Vertical and horizontal map textures*/

	ra = fixAng(pa - DR * 30); /*Ray angle is the same as the player angle*/

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
			if (mp >= 0 && mp < mapX*mapY && mapW[mp] > 0) /*The mapW point hit a wall*/
			{
				hmt = mapW[mp] - 1;
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
			if (mp >= 0 && mp < mapX*mapY && mapW[mp] > 0) /*The mapW point hit a wall*/
			{
				vmt = mapW[mp] - 1;
				dof = 8; /*Shut the loop down*/
				vx = rx;
				vy = ry;
				disV = dist(px, py, vx, vy, ra);
			}
			else /*If the ray doesn't hit a wall*/
				{rx += xo; ry += yo, dof += 1;} /*Find the next vertical line*/
		}

		float shade;
		if (disV < disH) /*Vertical wall was hit*/
		{
			rx = vx;
			ry = vy;
			disT = disV;
			hmt = vmt;
			shade = 0.5;
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		}
		if (disH < disV) /*Horizontal wall was hit*/
		{
			rx = hx;
			ry = hy;
			disT = disH;
			shade = 1;
			SDL_SetRenderDrawColor(renderer, 0, 130, 0, 255);
		}

		int rt = 8; /*Set the desired ray thickness*/
		DrawThickLine(renderer, px, py, rx--, ry--, rt);

		/*Draw the 3D walls*/
		float ca = fixAng(pa - ra);
		disT = disT*cos(ca); /*This is to fix the fisheye effect*/
		float lineH = (tileSize*320) / disT;
		float ty_step = 32.0 / (float) lineH;
		float ty_off = 0;
		if (lineH > 320)
		{
			ty_off = (lineH - 320)/2.0;
			lineH = 320; /*Line/Wall height*/
		}
		float lineO = 160 - lineH / 2; /*Line offset*/

		/*-------------DRAW WALLS--------------------------------------------*/
		float ty = ty_off * ty_step + hmt * 32, tx;
		if (shade == 1)
		{
            tx = (int)(rx/2.0) % 32;
			if (ra < PI)
				tx = 31 - tx;
		}
        else
		{
            tx = (int)(ry/2.0) % 32;
			if (ra > P2 && ra < P3)
				tx = 31 - tx;
		}
		//ty += 32; /*To start with brick walls instead of check*/

		for (int y = 0; y < lineH; y++)
		{
			int tex_index = ((int)ty * 32) + (int)tx;
			float c = All_Textures[tex_index] * shade;
			//ApplyShading(c, hmt, shade);
			if (hmt == 0) /*Make check texture red*/
				SDL_SetRenderDrawColorF(renderer, c, c/2, c/2, 255);
			if (hmt == 1) /*Make brick yellow*/
				SDL_SetRenderDrawColorF(renderer, c, c, c/2, 255);
			if (hmt == 2) /*Make doors blue*/
				SDL_SetRenderDrawColorF(renderer, c/2, c/2, c, 255);
			if (hmt == 3) /*Make windows green*/
				SDL_SetRenderDrawColorF(renderer, c, c/2, c, 255);

			for (int i = -rt/2; i <= rt/2; i++) /*Loop to draw the walls*/
				SDL_RenderDrawPoint(renderer, r * 8+530 + i, lineO + y);

			ty += ty_step;
		}

		/*-------------DRAW FLOORS-------------------------------------------*/
		for (int y = lineO+lineH; y < 320; y++)
		{
			float dy = y - (320 / 2.0);
			float deg = pa * DR, raFix = cos(fixAng(pa - ra) * DR);
			tx = px/2 + cos(deg)*158*32/dy/raFix;
			ty = py/2 - sin(deg)*158*32/dy/raFix;
			int mp = mapF[(int)(ty/32.0)*mapX+(int)(tx/32.0)] *32 *32;
			float c = All_Textures[((int)(ty) &31)*32 + ((int)(tx)&31) + mp] * 0.7;
			//ApplyShading(c, 0, 0);
			SDL_SetRenderDrawColorF(renderer, c, c, c, 1);
			for (int i = -rt/2; i <= rt/2; i++) /*Loop to draw the walls*/
				SDL_RenderDrawPoint(renderer, r * 8+530 + i, y);

		/*-------------DRAW ROOF---------------------------------------------*/
			mp = mapC[(int)(ty/32.0)*mapX+(int)(tx/32.0)] *32 *32;
			c = All_Textures[((int)(ty) &31)*32 + ((int)(tx)&31) + mp] * 0.7;
			SDL_SetRenderDrawColorF(renderer, c, c, c, 1);
			for (int i = -rt/2; i <= rt/2; i++) /*Loop to draw the walls*/
				SDL_RenderDrawPoint(renderer, r * 8+530 + i, 320 - y);
		}


		ra += DR;
		ra = fixAng(ra);
	}
}
/*---------------------------------------------------------------------------*/

/*-----------------MANAGE INPUT----------------------------------------------*/
int old_wall = -1;
int door_open = 0;

void ButtonDown(SDL_KeyCode key)
{
	int xo = 0; if (pdx < 0) xo = -25; else xo = 25;
	int yo = 0; if (pdy < 0) yo = -25; else yo = 25;
	int ipx = px/64.0, ipx_add_xo = (px+xo)/64.0, ipx_sub_xo = (px-xo)/64.0;
	int ipy = py/64.0, ipy_add_yo = (py+yo)/64.0, ipy_sub_yo = (py-yo)/64.0;
	int front_index = ipy_add_yo * mapX + ipx_add_xo;
	int back_index = ipy_sub_yo * mapX + ipx_sub_xo;

	if (key == SDLK_UP || key == SDLK_w) /*Move forward*/
		keys.w = 1;
	if (key == SDLK_DOWN || key == SDLK_s) /*Move backwards*/
		keys.s = 1;
	if (key == SDLK_LEFT || key == SDLK_a) /*Turn left*/
		keys.a = 1;
	if (key == SDLK_RIGHT || key == SDLK_d) /*Turn right*/
		keys.d = 1;
	if (key == SDLK_o) /*Open and close doors*/
	{
		//printf("front_index: %d\npx: %f\npy: %f\n", front_index, px, py);
		if (mapW[front_index] == 2) /*Open the door*/
		{
			mapW[front_index] = 0;
			old_wall = front_index; /*Save the doors location*/
			door_open = 1; /*Set the open door flag*/
		}
	}
	if (!door_open) /*In case you get stuck in the wall*/
		if ((back_index != old_wall && front_index == old_wall) || (back_index == old_wall && front_index != old_wall))
		{
			mapW[old_wall] = 0; /*Reopen the wall*/
			door_open = 1;
		}
	if (door_open)
		if (back_index != old_wall && front_index != old_wall)
		{
			mapW[old_wall] = 2; /*Restore the door*/
			door_open = 0;
		}

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
/*---------------------------------------------------------------------------*/

/*-----------------------INIT PLAYER AND MAPS--------------------------------*/
void display(void)
{
	/*Frames per second, implement this later*/
	frame2 = SDL_GetPerformanceCounter();
	float elapsedTime = (frame2 - frame1) / (float)SDL_GetPerformanceFrequency();
	fps = 1.0f / elapsedTime;
	frame1 = frame2;

	/*Buttons*/
	//float movementSpeed = 25.0f * elapsedTime;
	if (keys.a) /*Turn left*/
	{
		pa -= 0.1f;
		pa = fixAng(pa);
		pdx = cos(pa) * 5;
		pdy = sin(pa) * 5;
	}
	if (keys.d) /*Turn right*/
	{
		pa += 0.1;
		pa = fixAng(pa);
		pdx = cos(pa) * 5;
		pdy = sin(pa) * 5;
	}

	/*Variables required to stop player from moving forward and backward through walls*/
	int xo = 0; if (pdx < 0) xo = -20; else xo = 20;
	int yo = 0; if (pdy < 0) yo = -20; else yo = 20;
	int ipx = px/64.0, ipx_add_xo = (px+xo)/64.0, ipx_sub_xo = (px-xo)/64.0;
	int ipy = py/64.0, ipy_add_yo = (py+yo)/64.0, ipy_sub_yo = (py-yo)/64.0;

	if (keys.w) /*Move forward*/
	{
		if (mapW[ipy * mapX + ipx_add_xo] == 0)
			px += pdx;
		if (mapW[ipy_add_yo * mapX + ipx] == 0)
			py += pdy; /*Extra movement values*/
	}
	if (keys.s) /*Move backwards*/
	{
		if (mapW[ipy * mapX + ipx_sub_xo] == 0)
			px -= pdx; /*Extra movement values*/
		if (mapW[ipy_sub_yo * mapX + ipx] == 0)
			py -= pdy; /*Extra movement values*/
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
	pa = 0;	pdx =  cos(pa) * 5; pdy =  sin(pa) * 5;

	return (0);
}
/*---------------------------------------------------------------------------*/

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
