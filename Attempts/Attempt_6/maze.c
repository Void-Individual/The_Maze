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

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *screen_surface = NULL;

int mapX = 8, mapY = 8, tileSize = 64;
int map[] =
{
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 1, 1, 1, 1,
	1, 1, 0, 0, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};

void drawMap2D(void)
{
	int x, y, xo, yo;

	for (y = 0; y < mapY; y++)
	{
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
}

float dist(float ax, float ay, float bx, float by, float ang)
{
	(void) ang;
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

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
		//SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		//SDL_RenderDrawLine(renderer, px, py, rx, ry);

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
			SDL_SetRenderDrawColor(renderer, 220, 0, 0, 255);
		}
		if (disH < disV) /*Horizontal wall was hit*/
		{
			rx = hx;
			ry = hy;
			disT = disH;
			SDL_SetRenderDrawColor(renderer, 170, 0, 0, 255);
		}

		int rt = 8; /*Set the desired ray thickness*/
		//for (int i = -rt/2; i <= rt/2; i++)
		//{
		//	SDL_RenderDrawLine(renderer, px + i, py, rx + i, ry);
		//	SDL_RenderDrawLine(renderer, px, py + i, rx, ry + i);
		//}
		DrawThickLine(renderer, px, py, rx--, ry--, rt);

		/*Draw the 3D walls*/
		float ca = pa - ra;
		if (ca < 0) ca += 2 * PI;
		if (ca > 2 * PI) ca -= 2 * PI;
		disT = disT*cos(ca); /*This is to fix the fisheye effect*/
		float lineH = (tileSize*320) / disT;
		if (lineH > 320) lineH = 320; /*Line/Wall height*/
		float lineO = 160 - lineH / 2; /*Line offset*/
		for (int i = -rt/2; i <= rt/2; i++)
		{
			SDL_RenderDrawLine(renderer, r * 8+530 + i, lineO, r *8+530 + i, lineH+lineO);
			//SDL_RenderDrawLine(renderer, r * 8+530, lineO + i, r *8+530, lineH+lineO + i);
		}
		ra += DR;
		if (ra < 0) ra += 2 * PI;
		if (ra > 2 * PI) ra -= 2 * PI;
	}
}

void drawPlayer(void)
{
	/*To prevent overgrowth and under growth*/
	if (p_size < 5)		p_size = 5;
	if (p_size > 100)	p_size = 100;
	/*To prevent player from walking out of the screen*/
	if (px < 1)		px = 1;
	if (py < 1)		py = 1;
	if (px > SCREEN_WIDTH)		px = SCREEN_WIDTH - 1;
	if (py > SCREEN_HEIGHT)		py = SCREEN_HEIGHT - 1;
	p_px = (px + px + p_size) / 2;
	p_py = (py + py + p_size) / 2;

	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_Rect rect = { px - p_size / 2, py - p_size / 2, p_size, p_size};
    SDL_RenderFillRect(renderer, &rect);

	SDL_RenderDrawLine(renderer, px, py, px+pdx*5, py+pdy*5);
}

int poll_events(void)
{
	SDL_Event event; /*Holds events from SDL event queue*/
	SDL_KeyboardEvent key; /*Holds keyboard-specific events*/

	while (SDL_PollEvent(&event)) /*Loop to check through any pending events*/
	{
		switch (event.type)
		{
			case SDL_QUIT:
				return (1);
			case SDL_KEYDOWN:
				key = event.key;
				/*If escape was pressed*/
				if (key.keysym.sym == SDLK_ESCAPE)
					return (1);
				else
					switch (key.keysym.sym)
					{
						case SDLK_UP:
						case SDLK_w:
							px += pdx; 	py += pdy;
							/*Move up*/ break;
						case SDLK_DOWN:
						case SDLK_s:
							py -= pdy;	px -= pdx;
							/*Move down*/ break;
						case SDLK_LEFT:
						case SDLK_a:
							pa -= 0.1;
							if (pa < 0)		pa += 2*PI;
							pdx = cos(pa) * 5; pdy = sin(pa) * 5;
							/*Move left*/ break;
						case SDLK_RIGHT:
						case SDLK_d:
							pa += 0.1;
							if (pa > 2*PI)		pa -= 2*PI;
							pdx = cos(pa) * 5; pdy = sin(pa) * 5;
							/*Move right*/ break;
						case SDLK_b:
							p_size += 2;
							/*Increase player size*/ break;
						case SDLK_l:
							p_size -= 2;
							/*Reduce player size*/ break;
					}
		}
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
	px = 300; py = 300;
	/*Declare the players starting angles*/
	pa = 0;	pdx = cos(pa) * 5; pdy = sin(pa) * 5;

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
		drawMap2D();
		drawRays2D();
		drawPlayer();
		if (poll_events() == 1)
			break;
		SDL_RenderPresent(renderer); // Updtae the screen

	}

	return (0);
}
