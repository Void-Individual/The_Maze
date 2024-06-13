#include "maze.h"
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

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *screen_surface = NULL;

float px,py,pdx,pdy,pa;
float movementSpeed = 0.08;
float rotationSpeed = 0.06;

Uint32 frame1,frame2;
float fps;

int old_wall = -1; /*Give the old wall an invalid value*/
int door_open = 0; /*Bool condition for toggling door*/
int door = 3; /*The door tile value*/
int temp_door = 4; /*If the door has been opened, replace with this instead*/

int gameState = 0, timer = 0;
float fade = 0;

sprite sp[4]; /*This declares 4 sprites*/
ButtonKeys keys;
int depth[120];

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
}

/**
 * displayWelcomeImage - Function to display welcome image and instructions
 * Return: Nothing
*/

int displayWelcomeImage()
{
	SDL_Event event; /*Holds events from SDL event queue*/
    SDL_Texture* texture = loadTexture("src/images/welcome.png");
    if (texture == NULL) {
        printf("Failed to load welcome image!\n");
        return (0);
    }

    //SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

	while (SDL_WaitEvent(&event))
	{
		if (event.type == SDL_QUIT)
			return (1);
		if (event.key.keysym.sym == SDLK_SPACE)
			return (2);
	}

    //SDL_Delay(5000); /*Wait 5 seconds*/
    SDL_DestroyTexture(texture);
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

/**
 * main - Function to run a maze game
 * Return: 0
*/

int main(void)
{
	int running = 1;
    int imgFlags;
	if (init() != 0)
	{
		printf("Failed to init\n");
		return (1);
	}

	imgFlags = IMG_INIT_PNG; /*Initialize PNG loading*/
 	if( !( IMG_Init( imgFlags ) & imgFlags ) )
        printf( "PNG image could not initialize! Error: %s\n", IMG_GetError());
	else
		while (1)
			if (displayWelcomeImage() == 1)
			{
				running = 0;
				break;
			}
			else if (displayWelcomeImage() == 2)
				break;

	frame1 = SDL_GetTicks();
	while (running)
	{
		SDL_SetWindowPosition(window, SCREEN_WIDTH/2-850/2, SCREEN_HEIGHT/2-550/2);
		//Initialize renderer color
	    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		display();
		if (poll_events() == 1)
			running = 0;
		SDL_RenderPresent(renderer); // Update the screen

	}

	/*Cleanup*/
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return (0);
}

