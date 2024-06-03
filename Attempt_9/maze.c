#include "stdio.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "textures/sky.ppm"
#include "textures/win.ppm"
#include "textures/All_Textures.ppm"

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

float distance(float ax, float ay, float bx, float by, float ang)
{
	return (cos(degToRad(ang))*(bx-ax)-sin(degToRad(ang))*(by-ay));
}

float px,py,pdx,pdy,pa;
float movementSpeed = 0.05;
float rotationSpeed = 0.03;
Uint32 frame1,frame2;
float fps;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *screen_surface = NULL;

int SCREEN_WIDTH = 960;
int SCREEN_HEIGHT = 640;

int old_wall = -1; /*Give the old wall an invalid value*/
int door_open = 0; /*Bool condition for toggling door*/
int door = 4; /*The door tile value*/
int temp_door = 3; /*If the door has been opened, replace with this instead*/

typedef struct ButtonKeys {
	int w, a, s, d, o;
}ButtonKeys; /*Struct for the movement controls and directions*/
ButtonKeys keys;

int mapW[]= {          /*walls*/
	1, 1, 1, 1, 1, 1, 1, 1,
	5, 0, 0, 1, 0, 0, 0, 1,
	1, 0, 0, 4, 0, 2, 0, 1,
	1, 1, 4, 1, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 2, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};

int mapF[]= {          /*floors*/
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 3, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 3, 0, 0, 0, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};

int mapC[]= {          /*ceiling*/
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0,
	0, 1, 3, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};

/*-------------CUSTOM SDL2 FUNCTIONS-----------------------------------------*/
void SDL_RenderDrawLargePoint(int size, int x, int y)
{
	SDL_Rect rect = {x - size/2, y - size/2, size, size};
	SDL_RenderFillRect(renderer, &rect);
}

void SDL_RenderDrawLargerPoint(int size, int x, int y)
{
	SDL_Rect rect = {x*size, y*size, size, size};
	SDL_RenderFillRect(renderer, &rect);
}

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
/*---------------------------------------------------------------------------*/

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
				int ty = mapW[y*mapX+x]*32*32*3;
				//ty -= 32; /*Shift the textures up by one*/
				for (a = 1; a < 63; a+=2)
					for (b = 1; b < 63; b+=2)
					{

						int pixel = ((a/2) % 32 * 32 + (b/2) % 32)*3 + ty; // Repeat texture
						int red = All_Textures[pixel+0];
						int green = All_Textures[pixel+1];
						int blue = All_Textures[pixel+2];
						SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
						SDL_Rect rect = {xo+b, yo+a, 2, 2};
            		    SDL_RenderFillRect(renderer, &rect);
					}
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 165, 42, 42, 255); // Brown color
				SDL_Rect rect = {xo, yo, mapS-1, mapS-1};
                SDL_RenderFillRect(renderer, &rect);
			}
		}
}

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

void screen(int v)
{
	int x, y;
	int *T;
	//if (v == 1)
	//	T = sky;
	if (v == 2)
		T = win;
	//if (v == 3)
	//	T = lose;
	for (y = 0; y < 80; y++)
		for (x = 0; x < 120; x++)
		{
			int pixel = (y*120+x)*3;
			int red = T[pixel+0];
			int green = T[pixel+1];
			int blue = T[pixel+2];
			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
			SDL_RenderDrawLargePoint(8, x*8, y*8);
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
	SDL_RenderDrawLargePoint(8, px, py);

	/*Small directional pointer*/
	//SDL_RenderDrawLine(renderer, px, py, px+pdx*10, py+pdy*10);
}

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
		/*We dont need the 2D rays anymore*/
		//SDL_RenderDrawThickLine(px, py, rx, ry, 2);
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
			int pixel = ((int)ty*32 + (int)tx)*3 + (hmt*32*32*3); /*Add hmt to retrieve the right texture per tile*/
			int red = All_Textures[pixel+0]*shade;
			int green = All_Textures[pixel+1]*shade;
			int blue = All_Textures[pixel+2]*shade;
			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
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

			int pixel = ((((int)ty & 31)* 32) + ((int)tx & 31)) * 3 + mp * 3;
			int red = All_Textures[pixel+0] * 0.7;
			int green = All_Textures[pixel+1] * 0.7;
			int blue = All_Textures[pixel+2] * 0.7;
			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
			SDL_RenderDrawLargePoint(8, r * 8, y);

			/*-----Draw Ceiling------*/
			mp = mapC[(int)(ty/32.0)*mapX+(int)(tx/32.0)] *32 *32;
			pixel = ((((int)ty & 31)* 32) + ((int)tx & 31)) * 3 + mp * 3;
			red = All_Textures[pixel+0];
			green = All_Textures[pixel+1];
			blue = All_Textures[pixel+2];
			if (mp > 0) /*Only draw the ceiling if it has a value*/
			{
				SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
				SDL_RenderDrawLargePoint(8, r * 8, 640 - y);
			}
		}


		/*-------------------------------------------------------------------*/

		ra=FixAng(ra-0.5); /*Move each ray back by half an angle now*/
	}

}

void display(void)
{
	/*Frames per second*/
	frame2 = SDL_GetTicks(); // Get current time in milliseconds again
	fps = frame2 - frame1; // Calculate time difference between frames
	frame1 = SDL_GetTicks();
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
			px -= pdx*0.2*fps;
		if (mapW[ipy_sub_yo * mapX + ipx] == 0)
			py -= pdy*0.2*fps;
	}

	/*We dont need this for now*/
	//drawMap2D();
	//drawPlayer();
	drawSky();
	drawRays();
	//screen(2);
}

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
	if (key == SDLK_o) /*Open and close doors*/
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
	px = 150; py = 400;
	/*Declare the players starting angles*/
	pa = 90;
	pdx = cos(degToRad(pa));
	pdy = -sin(degToRad(pa));

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
		SDL_SetWindowPosition(window, SCREEN_WIDTH/2-960/2, SCREEN_HEIGHT/2-640/2);
		//Initialize renderer color
	    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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
