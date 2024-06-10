#include "maze.h"

/**
 * close_SDL - Function to release SDL instance
 * Return: empty
*/

void close_SDL(void)
{
	//Deallocate surfaces
	for( int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i )
	{
		SDL_FreeSurface( gKeyPressSurfaces[ i ] );
		gKeyPressSurfaces[ i ] = NULL;
	}
    SDL_DestroyTexture(loadedTexture); //Free loaded image
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

/**
 * main - Functional Key Presses
 * Return: 0
*/

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *windowsurface = NULL;
SDL_Texture* loadedTexture = NULL;

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
//Button constants
int BUTTON_WIDTH = 300;
int BUTTON_HEIGHT = 200;
int TOTAL_BUTTONS = 4;
// Define the array of surfaces
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface* gCurrentSurface = NULL; // Define the current surface

int main(void)
{
	int quit = 0;

	if (init_instance() != 0) /*Initialize the SDL instance*/
		return (1);

	/*Get window surface before loading media*/
	windowsurface = SDL_GetWindowSurface(window);
	//Load media
	if(!loadMedia())
		printf( "Failed to load media!\n" );

	/**Set default surface*/
	gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
	while (!quit) /*DON'T Set up an infinite loop that you cant break*/
	{
		windowsurface = SDL_GetWindowSurface(window);
		/*Blit the image onto the window surface*/
		quit = poll_events();
		//Apply the image stretched
		SDL_Rect stretchRect;
		stretchRect.x = 0;
		stretchRect.y = 0;
		stretchRect.w = SCREEN_WIDTH;
		stretchRect.h = SCREEN_HEIGHT;
		SDL_BlitScaled(gCurrentSurface, NULL, windowsurface, &stretchRect);

		// SDL_RenderClear(renderer); // Clear the screen
		// Render texture to screen
		// SDL_RenderCopy(renderer, loadedTexture, NULL, NULL);

		/*QUeue the surface*/
		//SDL_BlitSurface(gCurrentSurface, NULL, windowsurface, NULL);

		/*Update the surface*/
		SDL_UpdateWindowSurface(window);
		//draw_stuff();
		//SDL_RenderPresent(renderer); // Updtae the screen
	}

	/*Hack to get window to stay up (Repurposed for c instead of c++)*/
    /*SDL_Event e; int quit = 0; while( quit == 0 ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = 1; } }*/

	close_SDL(); /*Function to release and free the SDL instance*/
    return (0);
}
