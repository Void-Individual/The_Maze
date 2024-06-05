#include "maze.h"

void startGame(SDL_Instance *instance)
{
	/*Variables required to stop player from moving forward and backward through walls*/
	int xo = 0, yo = 0;

	if (instance->pdx < 0)
		xo = -20;
	else
		xo = 20;
	if (instance->pdy < 0)
		yo = -20;
	else
		yo = 20;
	int ipx = instance->px / 64.0, ipx_add_xo = (instance->px + xo) / 64.0;
	int ipx_sub_xo = (instance->px - xo) / 64.0;
	int ipy = instance->py / 64.0, ipy_add_yo = (instance->py + yo) / 64.0;
	int ipy_sub_yo = (instance->py - yo) / 64.0;

	/*Buttons*/
	if (instance->keys.a) /*Turn left*/
	{
		instance->pa += instance->rotationSpeed * instance->fps;
		instance->pa = FixAng(instance->pa);
		instance->pdx = cos(degToRad(instance->pa));
		instance->pdy = -sin(degToRad(instance->pa));
	}
	if (instance->keys.d) /*Turn right*/
	{
		instance->pa -= instance->rotationSpeed * instance->fps;
		instance->pa = FixAng(instance->pa);
		instance->pdx = cos(degToRad(instance->pa));
		instance->pdy = -sin(degToRad(instance->pa));
	}
	if (instance->keys.w) /*Move forward*/
	{
		if (instance->mapW[ipy * mapX + ipx_add_xo] == 0)
			instance->px += instance->pdx * instance->movementSpeed * instance->fps;
		if (instance->mapW[ipy_add_yo * mapX + ipx] == 0)
			instance->py += instance->pdy * instance->movementSpeed * instance->fps;
	}
	if (instance->keys.s) /*Move backwards*/
	{
		if (instance->mapW[ipy * mapX + ipx_sub_xo] == 0)
			instance->px -= instance->pdx * instance->movementSpeed * instance->fps;
		if (instance->mapW[ipy_sub_yo * mapX + ipx] == 0)
			instance->py -= instance->pdy * instance->movementSpeed * instance->fps;
	}

	/*We dont need this for now*/
	/*drawMap2D();*/
	/*drawPlayer();*/
	drawSky(instance);
	drawRays(instance);
	drawSprite(instance);

	/*If the player gets to the specified win tile, They win, end the game*/
	if ((int)instance->px >> 6 == 1 && (int)instance->py >> 6 == 1)
	{
		instance->fade = 0;
		instance->timer = 0;
		instance->gameState = 3;
	}
}

void init_game(SDL_Instance *instance)
{
	/*Declare the players starting point*/
	instance->px = 150;
	instance->py = 400;
	/*Declare the players starting angles*/
	instance->pa = 90;
	instance->pdx = cos(degToRad(instance->pa));
	instance->pdy = -sin(degToRad(instance->pa));
	/*Reset the doors*/
	instance->mapW[19] = 3;
	instance->mapW[26] = 3;

	/*Init sprite 1*/
	instance->sp[0].type = 1;
	instance->sp[0].state = 1;
	instance->sp[0].map = 0;
	instance->sp[0].x = 1.5 * 64;
	instance->sp[0].y = 5 * 64;
	instance->sp[0].z = 20; /*Height variable*/
}

void init_gamestate(SDL_Instance *instance)
{
	if (instance->gameState == 0) /*Init the game*/
	{
		instance->timer = 0;
		instance->fade = 0;
		instance->gameState = 1;
		init_game(instance);
	}
	if (instance->gameState == 1) /*Start scren*/
	{
		drawScreen(instance, 1);
		instance->timer += 1 * instance->fps;
		if (instance->timer > 3000)
		{
			instance->fade = 0;
			instance->timer = 0;
			instance->gameState = 2;
		}
	}
	if (instance->gameState == 2) /*Start the game*/
		startGame(instance);
	if (instance->gameState == 3) /*If you win, show win screen*/
	{
		drawScreen(instance, 2);
		instance->timer += 1 * instance->fps;
		if (instance->timer > 3000)
		{
			instance->fade = 0;
			instance->timer = 0;
			instance->gameState = 0;
		}
	}
}
