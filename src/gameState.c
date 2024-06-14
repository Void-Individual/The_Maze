#include "maze.h"

void stageZero(void)
{
	mapW = stage_0_w;
	mapF = stage_0_f;
	mapC = stage_0_c;

	/*Reset the doors*/
	//mapW[19] = 3;
	//mapW[26] = 3;
	door_open = 0;
	old_wall = -1;
	unlock = 1;

	/*Declare the players starting point*/
	px = 4 * 64; py = 4 * 64;
	/*Declare the players starting angles*/
	pa = 90;
	pdx = cos(degToRad(pa));
	pdy = -sin(degToRad(pa));

	/*Deactivate all sprites*/
	sp[0].state = 0;
	sp[1].state = 0;
	sp[2].state = 0;
	sp[3].state = 0;
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
		mapSprites();
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
		init_stage();
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
		if (timer > 50000) /*you lose the game after 50 seconds*/
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
			stage++;
			if (stage > 2)
				stage = 0;
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
