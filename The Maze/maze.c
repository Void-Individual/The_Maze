#include "stdio.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#include "textures/sky.ppm"
#include "textures/start.ppm"
#include "textures/win.ppm"
#include "textures/win_tile.ppm"
#include "textures/lose.ppm"
#include "textures/open_door.ppm"
#include "textures/closed_door.ppm"
#include "textures/floor_textures.ppm"
#include "textures/ceiling_textures.ppm"
#include "textures/map_textures.ppm"
#include "textures/Textures5.ppm"
#include "textures/red_brick.ppm"
#include "textures/small_red_bricks.ppm"
#include "textures/blue_tile_wall.ppm"
#include "textures/metal_surface.ppm"
#include "textures/enemy.ppm"
#include "textures/key.ppm"
#include "textures/babe.ppm"
#include "textures/toilet_guy.ppm"
#include "textures/enemy2.ppm"
#include "textures/snow.ppm"
#include "textures/water.ppm"

#define mapX  8     /*map width*/
#define mapY  8      /*map height*/
#define mapS 64      /*map cube size*/

float degToRad(float a)
{
	return (a*M_PI/180.0);
}

float FixAng(float a)
{
	if(a>359)
		a -= 360;
	if(a<0)
		a += 360;

	return (a);
}

float px,py,pdx,pdy,pa;
float movementSpeed = 0.08;
float rotationSpeed = 0.06;
Uint32 frame1,frame2;
float fps;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *screen_surface = NULL;

int SCREEN_WIDTH = 960;
int SCREEN_HEIGHT = 640;

int old_wall = -1; /*Give the old wall an invalid value*/
int door_open = 0; /*Bool condition for toggling door*/
int door = 3; /*The door tile value*/
int temp_door = 4; /*If the door has been opened, replace with this instead*/

int gameState = 0, timer = 0;
float fade = 0;

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
ButtonKeys keys;

/*Struct to handle every sprite*/
typedef struct
{
	int type; /*Is it static, a key or an enemy?*/
	int state; /*Is it on or off*/
	int map; /*The texture to be shown*/
	float x, y, z; /*The position*/
} sprite; sprite sp[4]; /*This declares 4 sprites*/
int depth[120];

int winTile = 1;
int mapW[]= {          /*walls*/
	1, 1, 1, 1, 1, 1, 1, 1,
	5, 0, 0, 1, 0, 0, 0, 1,
	1, 0, 0, 3, 0, 2, 0, 1,
	1, 1, 3, 1, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 2, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};

int mapF[]= {          /*floors*/
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 1, 0, 0, 0, 1, 0,
	0, 0, 1, 0, 0, 0, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};

int mapC[]= {          /*ceiling*/
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0,
	0, 1, 2, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};

/*-------------CUSTOM SDL2 FUNCTIONS-----------------------------------------*/
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

SDL_Surface *loadSurface(char *path)
{
	/*Load image at the specified path*/
	SDL_Surface *loadedsurface = NULL;
	SDL_Surface *optimizedSurface = NULL;

	if (strcmp(path, "../image/welcome.png") == 0)
		loadedsurface = IMG_Load(path);
	else
		loadedsurface = SDL_LoadBMP(path);

	if (loadedsurface == NULL)
		printf("Unable to load image %s\nError: %s\n", path, SDL_GetError());
	else /*convert surface to screen format*/
	{
		optimizedSurface = SDL_ConvertSurface(loadedsurface, screen_surface->format, 0);
		if (optimizedSurface == NULL)
			printf("Unable to optimize image %s!\n Error: %s\n", path, SDL_GetError());
		SDL_FreeSurface(loadedsurface); /*Get rid of old loaded surface*/
	}

	return (optimizedSurface);
}

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
/*---------------------------------------------------------------------------*/

/**
 * drawMap2D - Function to draw the 2D map of the game
 * Return: Nothing
*/

void drawMap2D(void)
{
	int x, y, xo, yo;

	for (y = 0; y < mapY; y++)
		for (x = 0; x < mapX; x++)
		{
			xo = x * mapS;
			yo = y * mapS;
			if (mapW[y * mapX + x] > 0)
			{
				int a, b;
				/*Adjust index to match texture array*/
				int ty = (mapW[y * mapX + x] - 1) * 32 * 32 * 3;

				for (a = 0; a < 64; a+=2)
					for (b = 0; b < 64; b+=2)
					{
						int pixel = (((a/2) % 32) * 32 + ((b/2) % 32))*3 + ty; // Repeat texture
						int red = map_textures[pixel+0];
						int green = map_textures[pixel+1];
						int blue = map_textures[pixel+2];
						SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
						SDL_Rect rect = {xo+b, yo+a, 2, 2};
            		    SDL_RenderFillRect(renderer, &rect);
					}
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 165, 42, 42, 255); // Brown color
				SDL_Rect rect = {xo, yo, mapS-1, mapS-1};
                //SDL_RenderFillRect(renderer, &rect);
                drawRectOutline(renderer, rect);
			}
		}
}

/**
 * drawSky - Function to draw the sky
 * Return: Nothing
*/


void drawSky(void)
{
	int x, y;
	for (y = 0; y < 40; y++) /*Because we never see below the horizon, we only need the top half*/
		for (x = 0; x < 120; x++)
		{
			/*To allow the sky to rotate as you turn, use player angle instead*/
			int xo = (int)pa*2 - x;
			if (xo < 0)
				xo += 120;
			xo = xo % 120;
			int pixel = (y*120+xo)*3;
			int red = sky[pixel+0];
			int green = sky[pixel+1];
			int blue = sky[pixel+2];
			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
			SDL_RenderDrawLargePoint(8, x*8, y*8);
		}
}

/**
 * drawScreen - Function to draw full screen pages
 * @v: Version of the screen to draw
 * Return: Nothing
*/

void drawScreen(int v)
{
	int x, y;
	int *T;
	if (v == 1)
		T = start;
	if (v == 2)
		T = win;
	if (v == 3)
		T = lose;
	for (y = 0; y < 80; y++)
		for (x = 0; x < 120; x++)
		{
			int pixel = (y*120+x)*3;
			int red = T[pixel+0]*fade;
			int green = T[pixel+1]*fade;
			int blue = T[pixel+2]*fade;
			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
			SDL_RenderDrawLargePoint(8, x*8, y*8);
		}
	/*Use fade value to slowly the intensity of display*/
	if (fade < 1)
		fade += 0.005*(fps/15);
	if (fade > 1)
		fade = 1;
//	printf("fade: %f, fps: %f\n", fade, fps);
}

/**
 * drawSprite - Function to draw sprites
 * Return: Nothing
*/

void *spriteTexture(int type)
{
	if (type == 1)
		return (key);
	if (type == 2)
		return (babe);
	if (type == 3)
		return (enemy);
	if (type == 4)
		return (enemy2);
	if (type == 5)
		return (toilet_guy);
}

void drawSprite(void)
{
	/*Make this sprite a key*/
	if (px < sp[0].x+30 && px > sp[0].x-30 && py < sp[0].y+30 && py > sp[0].y-30)
	sp[0].state = 0; /*If youre close to its position, pick it up*/
	/*If enemy comes close to player*/
	if (px < sp[3].x+30 && px > sp[3].x-30 && py < sp[3].y+30 && py > sp[3].y-30)
	{/*Lose the game and restart*/
		fade = 0;
		timer = 0;
		gameState = 4;
	}

	/*Enemy attack*/
	/*Normal grid position of sprite*/
	int spx = (int)sp[3].x >> 6, spy = (int)sp[3].y >> 6; 
	/*normal grid position plus offset*/
	int spx_add = ((int)sp[3].x + 15) >> 6, spy_add = ((int)sp[3].y + 15) >> 6;
	/*normal grid position minus offset*/
	int spx_sub = ((int)sp[3].x - 15) >> 6, spy_sub = ((int)sp[3].y - 15) >> 6;

	/*Stop the enemy from walking through walls*/
	if (sp[3].x > px && (mapW[spy * 8 + spx_sub] == 0 || mapW[spy * 8 + spx_sub] == temp_door)) /*If the player is to the left*/
		sp[3].x -= 0.03 * fps; /*Move to the left*/
	if (sp[3].x < px && (mapW[spy * 8 + spx_add] == 0|| mapW[spy * 8 + spx_add] == temp_door)) /*If the player is to the right*/
		sp[3].x += 0.03 * fps; /*Move to the right*/
	if (sp[3].y < py && (mapW[spy_add * 8 + spx] == 0 || mapW[spy_add * 8 + spx] == temp_door)) /*If the player is below the enemy*/
		sp[3].y += 0.03 * fps; /*Move downward*/
	if (sp[3].y > py && (mapW[spy_sub * 8 + spx] == 0 || mapW[spy_sub * 8 + spx] == temp_door)) /*If the player is above the enemy*/
		sp[3].y -= 0.03 * fps; /*Move to the left*/
	int x, y, s;
	for (s = 0; s < 4; s++) /*to cycle through all 4 sprites*/
	{
		/*This is done using a 3D projection matrix*/
		float sx = sp[s].x - px; /*Temp float variables*/
		float sy = sp[s].y - py;
		float sz = sp[s].z;

		/*Rotate the sprite around its origin*/
		float CS = cos(degToRad(pa)); /*sine and cosine rotation matrix*/
		float SN = sin(degToRad(pa));
		float a = sy * CS + sx * SN;
		float b = sx * CS - sy * SN;
		sx = a;
		sy = b;

		/* Prevent division by zero or near-zero values */
    	if (sy == 0)
			sy = 0.001;

		/*Convert it to screen x, y*/
		sx = (sx * 108.0/sy) + (120/2);
		sy = (sz * 108.0/sy) + (80/2);


			
		int x, y, scale= 32 * 80 / b; /*Variables to draw a line*/

		/* Check if the scale is valid */
    	if (scale < 0)
			scale = 0;
		if (scale > 120)
			scale = 120;
    
		/*Textures*/
		float t_x = 0, t_y = 31, t_y_step = 32.0 / (float)scale;
		float t_x_step = 31.5 / (float)scale;
		/*Draw a point*/
		for (x = sx - scale / 2; x < sx + scale / 2; x++)
		{
			t_y = 31;
			for (y = 0; y < scale; y++)
			{
				if (sp[s].state == 1 && x > 0 && x < 120 && b < depth[x])
				{
					int *Texture = spriteTexture(sp[s].type);
					int pixel = ((int)t_y*32 + (int)t_x)*3;// + (hmt*32*32*3); /*Add hmt to retrieve the right texture per tile*/
					int red = Texture[pixel+0];
					int green = Texture[pixel+1];
					int blue = Texture[pixel+2];
					if (red != 255, green != 0, blue != 255) /*Dont draw if purple*/
     				{
						SDL_SetRenderDrawColor(renderer, red, green, blue, 225);
						/*Only draw this sprite if its onscreen and if its depth is closer than the wall*/
						SDL_RenderDrawLargePoint(8, x * 8, sy * 8 - y * 8);
					}
					t_y -= t_y_step;
					if (t_y < 0)
						t_y = 0;
				}
			}
			t_x += t_x_step;
		}
	}
}

/**
 * drawPlayer - Function to draw the player on the 2D map
 * Return: Nothing
*/

void drawPlayer(void)
{
	/*To prevent player from walking out of the screen*/
	if (px < 1)		px = 1;
	if (py < 1)		py = 1;
	if (px > SCREEN_WIDTH)		px = SCREEN_WIDTH - 1;
	if (py > SCREEN_HEIGHT)		py = SCREEN_HEIGHT - 1;

	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_RenderDrawLargePoint(8, px, py);

	/*Small directional pointer*/
	//SDL_RenderDrawLine(renderer, px, py, px+pdx*10, py+pdy*10);
}

/**
 * selectTextures - 
 * @hmt: surface texture value
 * Return: pointer to an array of textures
 */

int *selectTextures(int hmt)
{
	/*Note that the hmt values are one behind the map values*/
	if (hmt == 0)
		return (red_brick);
	if (hmt == 1)
		return (small_red_bricks);
	if (hmt == 2)
		return (closed_door);
	if (hmt == 3)
		return (open_door);
	if (hmt == 4)
		return (win_tile);
	else
		return (metal_surface);
}

/**
 * setTextureColor: Function to set the rendere colour to rgb
 * @hmt: The surface texture value
 * @shade: the intensity of light to be drawn on the surface
 * @tx: the x axis of the surface to be textured
 * @ty: the y axis of the surface to be textured
 * @wall: if the surface is a wall, this var should be 1, else 0
 * @floor: if the surface is a floor, this var should be 1, else 0
 * @ceiling: if the surface is a ceiling, this var should be 1, else 0
 * @mp: this variable is only needed if either ceiling or floor is 1
 * Return: Nothing 
*/

void setTextureColor(int hmt, float shade, float tx, float ty, int wall, int floor, int ceiling, int mp)
{
	int pixel, red, green, blue, *Texture;
	if (floor)
	{
		pixel = ((((int)ty & 31)* 32) + ((int)tx & 31)) * 3 + mp * 3;
		red = floor_textures[pixel+0] * 0.7;
		green = floor_textures[pixel+1] * 0.7;
		blue = floor_textures[pixel+2] * 0.7;
		SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
	}
	if (ceiling)
	{
		pixel = ((((int)ty & 31)* 32) + ((int)tx & 31)) * 3 + mp * 3;
		red = ceiling_textures[pixel+0] * 0.7;
		green = ceiling_textures[pixel+1] * 0.7;
		blue = ceiling_textures[pixel+2] * 0.7;
		SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
	}
	if (wall)
	{
		Texture = selectTextures(hmt);
		pixel = ((int)ty*32 + (int)tx)*3;// + (hmt*32*32*3); /*Add hmt to retrieve the right texture per tile*/
		red = Texture[pixel+0]*shade;
		green = Texture[pixel+1]*shade;
		blue = Texture[pixel+2]*shade;
		SDL_SetRenderDrawColor(renderer, red, green, blue, 225);

		if (hmt == 2 || hmt == 3)
			if ((red == 255 && green == 255 && blue == 255) ||
				(red == 255*shade && green == 255*shade && blue == 255*shade))
				SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255);

	}
}

/**
 * drawRays - Function to draw the 2D pov rays on the map and
 * the entire 3D environment
 * Return: Nothing
*/

void drawRays(void)
{
	int r, mx, my, mp, dof;
	float vx, vy, rx, ry, ra, xo, yo, disV, disH;

	ra = FixAng(pa + 30); /*Set the ray back 30 degrees*/
	for (r = 0; r < 120; r++) /*Doubled the no of rays to increase fov*/
	{
		int vmt = 0, hmt = 0; /*Vertical and horizontal map textures*/

		/*Check Vertical lines*/
		dof = 0; disV = 100000;
		float Tan = tan(degToRad(ra));
       	if (cos(degToRad(ra)) > 0.001) /*Ray is looking left*/
		{
			rx = (((int)px >> 6) << 6) + 64;
			ry = (px - rx) * Tan + py;
			xo = 64; yo = -xo * Tan;
		}
  		else if (cos(degToRad(ra)) < -0.001) /*Ray is looking right*/
		{
			rx = (((int)px >> 6) << 6) - 0.0001;
			ry = (px - rx) * Tan + py;
			xo = -64; yo = -xo * Tan;
		}
 		else /* Ray is looking up or down and hit no walls*/
		{
			rx = px;
			ry = py;
			dof = 8;
		}

		while(dof<8)
  		{
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;
			mp = my * mapX + mx;
   			if (mp > 0 && mp < mapX * mapY && mapW[mp] > 0) /*A vertical wall has been hit*/
			{
				vmt = mapW[mp] - 1;
				dof = 8;
				disV = cos(degToRad(ra)) * (rx - px) - sin(degToRad(ra)) * (ry-py);
			}
		   	else /*Check the next vertical*/
			{
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}
		vx = rx; vy = ry;

		/*Check Horizontal lines*/
		dof=0; disH=100000;
		Tan=1.0/Tan;
       	if (sin(degToRad(ra)) > 0.001) /*The ray is looking up*/
		{
			ry = (((int)py >> 6) << 6) - 0.0001;
			rx = (py - ry) * Tan + px;
			yo = -64;
			xo = -yo * Tan;
		}
  		else if (sin(degToRad(ra)) < -0.001) /*The ray is looking down*/
		{
			ry = (((int)py >> 6) << 6) + 64;
			rx = (py - ry) * Tan + px;
			yo = 64;
			xo = -yo * Tan;
		}
  		else /*The ray is looking straight left or right*/
		{
			rx = px;
			ry = py;
			dof = 8;
		}

  		while (dof < 8)
  		{
	   		mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;
			mp = my * mapX + mx;
   			if (mp > 0 && mp < mapX * mapY && mapW[mp] > 0) /*A horizontal wall has been hit*/
			{
				hmt = mapW[mp] - 1;
				dof = 8;
				disH = cos(degToRad(ra)) * (rx - px) - sin(degToRad(ra))*(ry-py);
			}
   			else /*Check next horizontal*/
			{
				rx += xo;
				ry += yo;
				dof += 1;
			}

  		}
		/*----------Draw the 2D rays-----------------------------------------*/
		float shade=1;
		SDL_SetRenderDrawColorF(0, 0.8, 0, 1);
  		if (disV < disH) /*If a horizontal hit first, change these values*/
		{
			hmt = vmt;
			shade = 0.5;
			rx = vx;
			ry = vy;
			disH = disV;
			SDL_SetRenderDrawColorF(0, 0.6, 0, 1);
		}
		/*Only Show the 2D rays if tha map key was pressed*/
		if (keys.m)
			SDL_RenderDrawThickLine(px, py, rx, ry, 2);
		/*-------------------------------------------------------------------*/

		/*---------------Draw the 3D map-------------------------------------*/
		int ca = FixAng(pa-ra);
		disH = disH * cos(degToRad(ca)); /*To fix fisheye effects*/
		int lineH = (mapS * 640) / disH; /*Line height*/
		float ty_step = 32.0 / (float)lineH;
		float ty_off = 0;
		if (lineH > 640) /*Monitor the line height and limit it*/
		{
			ty_off = (lineH - 640)/2.0;
			lineH = 640;
		}
		int lineO = 320 - (lineH >> 1); /*Line offset*/

		depth[r] = disH; /*Save this lines depth*/

		/*----------Draw Walls------------------*/
		int y;
		float ty = ty_off * ty_step;// + hmt * 32;
		float tx;
		if (shade == 1) /*For the North and south walls*/
		{
			tx = (int)(rx / 2.0) % 32;
			if (ra > 180) /*Rotate the texture if the map is looking south*/
				tx = 31 - tx;
		}
		else /*For the east and west walls*/
		{
			tx = (int)(ry / 2.0) % 32;
			if (ra > 90 && ra < 270) /*Rotate the texture if the map is looking east*/
				tx = 31 - tx;
		}


		//ty += 32 * 3;

		for (y = 0; y < lineH; y++)
		{
			setTextureColor(hmt, shade, tx, ty, 1, 0, 0, 0);
			SDL_RenderDrawLargePoint(8, r * 8, y + lineO);
			ty += ty_step;
		}
		/*---------Draw floors-------*/
		for (y = lineO + lineH; y < 640; y++)
		{
			float dy = y - (640/2.0);
			float deg = degToRad(ra);
			float raFix = cos(degToRad(FixAng(pa - ra)));
			/*Added the *2 into tx and ty to prevent sliding*/
			tx = px/2 + cos(deg) * 158 * 2 * 32 / dy / raFix;
			ty = py/2 - sin(deg) * 158 * 2 * 32 / dy / raFix;
			int mp = mapF[(int)(ty/32.0)*mapX+(int)(tx/32.0)] *32 *32;

			setTextureColor(hmt, shade, tx, ty, 0, 1, 0, mp);
			SDL_RenderDrawLargePoint(8, r * 8, y);

			/*-----Draw Ceiling------*/
			mp = mapC[(int)(ty/32.0)*mapX+(int)(tx/32.0)] *32 *32;
			if (mp > 0) /*Only draw the ceiling if it has a value*/
			{
				setTextureColor(hmt, shade, tx, ty, 0, 0, 1, mp);
				SDL_RenderDrawLargePoint(8, r * 8, 640 - y);
			}
		}


		/*-------------------------------------------------------------------*/

		ra=FixAng(ra-0.5); /*Move each ray back by half an angle now*/
	}

}

/**
 * startGame - Function to start the visible game process
 * Return: Nothing
 */
 
void startGame(void)
{
	/*Buttons*/
	if (keys.a) /*Turn left*/
	{
		pa += rotationSpeed*fps;
		pa = FixAng(pa);
		pdx = cos(degToRad(pa));
		pdy = -sin(degToRad(pa));
	}
	if (keys.d) /*Turn right*/
	{
		pa -= rotationSpeed*fps;
		pa = FixAng(pa);
		pdx = cos(degToRad(pa));
		pdy = -sin(degToRad(pa));
	}

	/*Variables required to stop player from moving forward and backward through walls*/
	int xo = 0; if (pdx < 0) xo = -20; else xo = 20;
	int yo = 0; if (pdy < 0) yo = -20; else yo = 20;
	int ipx = px/64.0, ipx_add_xo = (px+xo)/64.0, ipx_sub_xo = (px-xo)/64.0;
	int ipy = py/64.0, ipy_add_yo = (py+yo)/64.0, ipy_sub_yo = (py-yo)/64.0;

	if (keys.w) /*Move forward*/
	{
		if (mapW[ipy * mapX + ipx_add_xo] == 0)
			px += pdx*movementSpeed*fps;
		if (mapW[ipy_add_yo * mapX + ipx] == 0)
			py += pdy*movementSpeed*fps;
	}
	if (keys.s) /*Move backwards*/
	{
		if (mapW[ipy * mapX + ipx_sub_xo] == 0)
			px -= pdx*movementSpeed*fps;
		if (mapW[ipy_sub_yo * mapX + ipx] == 0)
			py -= pdy*movementSpeed*fps;
	}

	/*We dont need this for now*/
	
	drawSky();
	drawRays();
	drawSprite();
	if (keys.m == 1) /*If the map key has been pressed*/
	{
		drawMap2D();
		drawPlayer();
	}

	/*If the player gets to the specified win tile, They win, end the game*/
	if ((int)px >> 6 == 1 && (int)py >> 6 == 1)
	{
		fade = 0;
		timer = 0;
		gameState = 3;
	}
}

/**
 * init_game - Function to declare the starting positions of everything
 * Return: Nothing
 */

void init_game(void)
{
	/*Declare the players starting point*/
	px = 150; py = 400;
	/*Declare the players starting angles*/
	pa = 90;
	pdx = cos(degToRad(pa));
	pdy = -sin(degToRad(pa));
	/*Reset the doors*/
	mapW[19] = 3;
	mapW[26] = 3;
	door_open = 0;
	old_wall = -1;

	/*Init sprite 1 as key*/
	sp[0].type = 1;
	sp[0].state = 1;
	sp[0].map = 0;
	sp[0].x = 1.5*64;
	sp[0].y = 5*64;
	sp[0].z = 20; /*Height variable*/

	/*Init sprite 2 as babe*/
	sp[1].type = 2;
	sp[1].state = 1;
	sp[1].map = 1;
	sp[1].x = 1.5*64;
	sp[1].y = 4.5*64;
	sp[1].z = 10; /*Height variable*/

	/*Init sprite 3 as toilet guy*/
	sp[2].type = 5;
	sp[2].state = 1;
	sp[2].map = 1;
	sp[2].x = 3.5*64;
	sp[2].y = 4.5*64;
	sp[2].z = 10; /*Height variable*/

	/*Init sprite 4 as enemy*/
	sp[3].type = 4;
	sp[3].state = 1;
	sp[3].map = 2;
	sp[3].x = 2*64;
	sp[3].y = 3*64;
	sp[3].z = 15; /*Height variable*/
}

/**
 * init_gamestate - Function to cycle the game through different run states
 * Return: Nothing
*/

void init_gamestate(void)
{
	if (gameState == 0) /*Init the game*/
	{
		timer = 0;
		fade = 0;
		gameState = 1;
		init_game();
	}
	if (gameState == 1) /*Start scren*/
	{
		drawScreen(1);
		timer += 1*fps;
		if (timer > 3000)
		{
			fade = 0;
			timer = 0;
			gameState = 2;
		}
	}
	if (gameState == 2) /*Start the game*/
	{
		timer += 1*fps;
		if (timer > 30000) /*you lose the game after 20 seconds*/
		{
			fade = 0;
			timer = 0;
			gameState = 4;
		}
		startGame();
	}

	if (gameState == 3) /*If you win, show win screen*/
	{
		drawScreen(2);
		timer += 1*fps;
		if (timer > 3000)
		{
			fade = 0;
			timer = 0;
			gameState = 0;
		}
	}
	if (gameState == 4) /*If you lose, show lose screen*/
	{
		drawScreen(3);
		timer += 1*fps;
		if (timer > 3000)
		{
			fade = 0;
			timer = 0;
			gameState = 0;
		}
	}
}

/**
 * display - func to calculate fps between loops before starting gamestate
 * Return: Nothing
*/

void display(void)
{
	/*Frames per second*/
	frame2 = SDL_GetTicks(); // Get current time in milliseconds again
	fps = frame2 - frame1; // Calculate time difference between frames
	frame1 = SDL_GetTicks();
	init_gamestate();
	//startGame();
}

/**
 * ButtonDown - Function to check which instance->keys have been pressed down
 * @key: Variable that contains the keycode
 * Return: Nothing
*/

void ButtonDown(SDL_KeyCode key)
{
	int xo = 0; if (pdx < 0) xo = -25; else xo = 25;
	int yo = 0; if (pdy < 0) yo = -25; else yo = 25;
	int ipx_add_xo = (px+xo)/64.0, ipx_sub_xo = (px-xo)/64.0;
	int ipy_add_yo = (py+yo)/64.0, ipy_sub_yo = (py-yo)/64.0;
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
	if (key == SDLK_m)
	{
		printf("Map activated\n");
		if (!keys.m)
			keys.m = 1;
		else
			keys.m = 0;
	}
		
	if (key == SDLK_o && sp[0].state == 0) /*Open and close doors*/
	{
		//printf("front_index: %d\npx: %f\npy: %f\n", front_index, px, py);
		if (mapW[front_index] == door) /*Open the door*/
		{
			if (old_wall) /*If there was a previously opened door*/
				mapW[old_wall] = door; /*Lock it back properly*/

			mapW[front_index] = 0;
			old_wall = front_index; /*Save the doors location*/
			door_open = 1; /*Set the open door flag*/
		}
		else if (front_index == old_wall)
		{
			mapW[front_index] = door;
			old_wall = -1;
			door_open = 0;
		}
	}

	if (!door_open) /*In case you get stuck in the wall*/
		if ((back_index != old_wall && front_index == old_wall) || (back_index == old_wall && front_index != old_wall))
		{
			mapW[old_wall] = 0; /*Reopen the wall*/
		 	door_open = 1;
		}
	if (door_open) /*Shut the door when the player isn't facing it*/
		if (back_index != old_wall && front_index != old_wall)
		{
			mapW[old_wall] = temp_door; /*Restore the door temporarily*/
			door_open = 0;
		}

}

/**
 * ButtonUp - Function to check the instance->keys that aren't pressed
 * @key: Variable that contains the keycode
 * Return: Nothing
*/

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

/**
 * poll_events - Function to register input data
 * Return: 0
*/

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

/**
 * init - Function to initialize SDL2, the window and the renderer
 * Return: Nothing
*/

int init(void)
{
	/* Initialize SDL*/
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Unable to Initialize SDL: %s\n", SDL_GetError());
		return (1);
	}

	/*Create a new window*/
	window = SDL_CreateWindow("Void's Maze Game \\o/",
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
	return (0);
}

void displayWelcomeImage() {
    SDL_Texture* texture = loadTexture("images/welcome.png");
    if (texture == NULL) {
        printf("Failed to load welcome image!\n");
        return;
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Delay(5000); // Wait 3 seconds

    SDL_DestroyTexture(texture);
}

int WinMain(void)
{
    int imgFlags;
	if (init() != 0)
	{
		printf("Failed to init\n");
		return (1);
	}

	imgFlags = IMG_INIT_PNG; //Initialize PNG loading
 	if( !( IMG_Init( imgFlags ) & imgFlags ) )
        printf( "PNG image could not initialize! Error: %s\n", IMG_GetError());
	else
		displayWelcomeImage();
	frame1 = SDL_GetTicks();
	while (1)
	{
		SDL_SetWindowPosition(window, SCREEN_WIDTH/2-850/2, SCREEN_HEIGHT/2-550/2);
		//Initialize renderer color
	    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		display();
		if (poll_events() == 1)
			break;
		SDL_RenderPresent(renderer); // Update the screen

	}
	/*Cleanup*/
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return (0);
}

