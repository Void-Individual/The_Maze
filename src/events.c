#include "maze.h"

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
