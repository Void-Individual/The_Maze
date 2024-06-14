#include "maze.h"

void stage_0(void)
{
	mapW = plain_walls;
	mapF = plain_floor;
	mapC = no_roof;

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


void stage_1(void)
{
	mapW = stage_0_w;
	mapF = plain_floor;
	mapC = stage_0_c;

	/*Reset the doors*/
 	//mapW[19] = 3;
	//mapW[26] = 3;
	door_open = 0;
	old_wall = -1;
	unlock = 1;

	/*Declare the players starting point*/
	px = 4.5 * 64; py = 4.5 * 64;
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
 * stage_2 - Function to declare the starting positions of everything
 * Return: Nothing
 */

void stage_2(void)
{
	mapW = stage_1_w;
	mapF = stage_1_f;
	mapC = stage_1_c;

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
	sp[0].x = 6.5*64;
	sp[0].y = 5.5*64;
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
	sp[3].x = 1.1*64;
	sp[3].y = 1.1*64;
	sp[3].z = 15; /*Height variable*/
}

/**
 * init_stage - Function to declare the starting positions of everything
 * Return: Nothing
*/

void init_stage(void)
{
	if (stage == 0)
		stage_0();
	else if (stage == 1)
		stage_1();
	else if (stage == 2)
		stage_2();
	else
		stage_0();
}
