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
				else
					switch (key.keysym.sym)
					{
						case SDLK_UP:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                    	    break;
                    	case SDLK_DOWN:
                    	    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                    	    break;
                    	case SDLK_LEFT:
                    	    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                	        break;
                	    case SDLK_RIGHT:
                	        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                	        break;
                	    default:
                	        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                	        break;
					}
		}
	}

	return (0);
}
