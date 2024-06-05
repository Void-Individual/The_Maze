#include "maze.h"

/**
 * toggleDoors - Function to open and close doors
 * @front_index: tile right in front of the player
 * @back_index: tile right behind the player
 * Return: Nothing
*/

void toggleDoors(SDL_Instance *instance, int front_index, int back_index)
{
	if (instance->mapW[front_index] == instance->door) /*Open the instance->door*/
	{
		if (instance->old_wall) /*If there was a previously opened instance->door*/
			instance->mapW[instance->old_wall] = instance->door; /*Lock it back properly*/

		instance->mapW[front_index] = 0;
		instance->old_wall = front_index; /*Save the doors location*/
		instance->door_open = 1; /*Set the open instance->door flag*/
	}
	else if (front_index == instance->old_wall) /*Lock the instance->door*/
	{
		instance->mapW[front_index] = instance->door;
		instance->old_wall = -1;
		instance->door_open = 0;
	}
}

/**
 * ButtonDown - Function to check which instance->keys have been pressed down
 * @key: Variable that contains the keycode
 * Return: Nothing
*/

void ButtonDown(SDL_Instance *instance, SDL_KeyCode key)
{
	int xo = 0;
	int yo = 0;

	if (instance->pdx < 0)
		xo = -25;
	else
		xo = 25;
	if (instance->pdy < 0)
		yo = -25;
	else
		yo = 25;

	int ipx_add_xo = (instance->px + xo) / 64.0, ipx_sub_xo = (instance->px - xo) / 64.0;
	int ipy_add_yo = (instance->py + yo) / 64.0, ipy_sub_yo = (instance->py - yo) / 64.0;
	int front_index = ipy_add_yo * mapX + ipx_add_xo;
	int back_index = ipy_sub_yo * mapX + ipx_sub_xo;

	if (key == SDLK_UP || key == SDLK_w) /*Move forward*/
		instance->keys.w = 1;
	if (key == SDLK_DOWN || key == SDLK_s) /*Move backwards*/
		instance->keys.s = 1;
	if (key == SDLK_LEFT || key == SDLK_a) /*Turn left*/
		instance->keys.a = 1;
	if (key == SDLK_RIGHT || key == SDLK_d) /*Turn right*/
		instance->keys.d = 1;
	if (key == SDLK_o) /*Open and close doors*/
		toggleDoors(instance, front_index, back_index);

	if (instance->door_open) /*In case you get stuck in the wall*/
		if ((back_index != instance->old_wall && front_index == instance->old_wall) ||
			(back_index == instance->old_wall && front_index != instance->old_wall))
			instance->mapW[instance->old_wall] = 0; /*Reopen the wall*/

	if (instance->door_open) /*Shut the instance->door when the player isn't facing it directly*/
		if (back_index != instance->old_wall && front_index != instance->old_wall)
			instance->mapW[instance->old_wall] = instance->temp_door; /*Restore the instance->door temporarily*/

}

/**
 * ButtonUp - Function to check the instance->keys that aren't pressed
 * @key: Variable that contains the keycode
 * Return: Nothing
*/

void ButtonUp(SDL_Instance *instance, SDL_Keycode key)
{
	if (key == SDLK_UP || key == SDLK_w) /*Move forward*/
		instance->keys.w = 0;
	if (key == SDLK_DOWN || key == SDLK_s) /*Move backwards*/
		instance->keys.s = 0;
	if (key == SDLK_LEFT || key == SDLK_a) /*Turn left*/
		instance->keys.a = 0;
	if (key == SDLK_RIGHT || key == SDLK_d) /*Turn right*/
		instance->keys.d = 0;
}

/**
 * poll_events - Function to register input data
 * Return: 0
*/

int poll_events(SDL_Instance *instance)
{
	SDL_Event event; /*Holds events from SDL event queue*/

	while (SDL_PollEvent(&event)) /*Loop to check through any pending events*/
	{
		if (event.type == SDL_QUIT)
			return (1);
		if (event.type == SDL_KEYDOWN)
			ButtonDown(instance, event.key.keysym.sym);
		if (event.type == SDL_KEYUP)
			ButtonUp(instance, event.key.keysym.sym);
	}

	return (0);
}
