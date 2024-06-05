#include "maze.h"
#include "textures/sky.ppm"
#include "textures/start.ppm"
#include "textures/win.ppm"
#include "textures/win_tile.ppm"
#include "textures/lose.ppm"
#include "textures/open_door.ppm"
#include "textures/closed_door.ppm"
#include "textures/Textures5.ppm"
#include "textures/red_brick.ppm"
#include "textures/small_red_bricks.ppm"
#include "textures/blue_tile_wall.ppm"
#include "textures/metal_surface.ppm"

/*-----------------Declare All Variables-----------------------------*/
void declareInstanceVariables(SDL_Instance *instance)
{
	int x;
	int mapW[] = {          /*walls*/
		1, 1, 1, 1, 1, 1, 1, 1,
		5, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 3, 0, 2, 0, 1,
		1, 1, 3, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 2, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
	};

	for (x = 0; x < 64; x++)
		instance->mapW[x] = mapW[x];


	int mapF[] = {          /*floors*/
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 1, 0, 0,
		0, 0, 0, 0, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 0, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};
	for (x = 0; x < 64; x++)
		instance->mapF[x] = mapF[x];

	int mapC[] = {          /*ceiling*/
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0,
		0, 1, 2, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};
	for (x = 0; x < 64; x++)
		instance->mapC[x] = mapC[x];

	/*Allocate Memory for 4 sprites*/
	instance->sp = (sprite *)malloc(4 * sizeof(sprite));
	if (instance->sp == NULL)
	{
		printf("Memory allocation for sprites failed\n");
		exit(1);
	}
	for (x = 0; x < 4; x++)
	{
		instance->sp[x].type = x;
		instance->sp[x].state = 0;
		instance->sp[x].map = 0;
		instance->sp[x].x = x * 10;
		instance->sp[x].y = x * 10;
		instance->sp[x].z = 0;
	}

	instance->movementSpeed = 0.07;
	instance->rotationSpeed = 0.05;

	instance->old_wall = -1; /*Give the old wall an invalid value*/
	instance->door_open = 0; /*Bool condition for toggling door*/
	instance->door = 3; /*The door tile value*/
	instance->temp_door = 4; /*If the door has been opened, replace with this instead*/

	instance->gameState = 0;
	instance->timer = 0;
	instance->fade = 0;
}

/*-------------------------------------------------------------------*/

void display(SDL_Instance *instance)
{
	/*Frames per second*/
	instance->frame2 = SDL_GetTicks(); /* Get current time in milliseconds again*/
	instance->fps = instance->frame2 - instance->frame1; /*Calculate time difference between frames*/
	instance->frame1 = SDL_GetTicks();
	init_gamestate(instance);
}


int init(SDL_Instance *instance)
{
	/* Initialize SDL*/
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Unable to Initialize SDL: %s\n", SDL_GetError());
		return (1);
	}

	/*Create a new instance->window*/
	instance->window = SDL_CreateWindow("Void's SDL2 \\o/",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
			SCREEN_HEIGHT, 0);


	if (instance->window == NULL)
	{
		fprintf(stderr, "Create instance->window error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}

	/*Create a new instance->renderer instance linked to the instance->window*/
	instance->renderer = SDL_CreateRenderer(instance->window, -1, SDL_RENDERER_ACCELERATED |
		SDL_RENDERER_PRESENTVSYNC);
	if (instance->renderer == NULL)
	{
		SDL_DestroyWindow(instance->window);
		fprintf(stderr, "Create instance->renderer error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	return (0);
}

int main(void)
{
	SDL_Instance *instance = NULL;

	instance = malloc(sizeof(SDL_Instance));
	if (instance == NULL)
	{
		free(instance);
		printf("Unable to allocate memory to instance\n");
		return (1);
	}

	if (init(instance) != 0)
	{
		printf("Failed to init\n");
		return (1);
	}

	declareInstanceVariables(instance);
	while (1)
	{
		SDL_SetWindowPosition(instance->window, SCREEN_WIDTH / 2 - 960 / 2,
			SCREEN_HEIGHT / 2 - 640 / 2);
		/*Initialize renderer color to light grey and pour it on screen*/
	    SDL_SetRenderDrawColor(instance->renderer, 30, 30, 30, 255);
		SDL_RenderClear(instance->renderer);
		display(instance);
		if (poll_events(instance) == 1)
			break;
		SDL_RenderPresent(instance->renderer); /*Updtae the screen*/

	}
	/*Cleanup*/
	SDL_DestroyRenderer(instance->renderer);
	SDL_DestroyWindow(instance->window);
	SDL_Quit();
	free(instance->sp);
	free(instance);
	return (0);
}
