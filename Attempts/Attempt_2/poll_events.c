#include "maze.h"

/**
 * poll_events - Function to control actions based on keys and exit
 * Return: 0 on success, else 1
*/

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
				if (key.keysym.scancode == 0x29)
					return (1);
				break;
		}
	}

	return (0);
}
