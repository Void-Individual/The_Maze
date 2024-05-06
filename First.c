#include <SDL2/SDL.h>

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        return (1);
    }

    // Create a new window instance
    window = SDL_CreateWindow("First stages", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 640, 480, 0);
	if (window == NULL)
	{
		fprintf(stderr, "Create window error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}

	// Create a new renderer instance linked to window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		fprintf(stderr, "Create renderer error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	//Hack to get window to stay up (Repurposed for c instead of c++)
    SDL_Event e; int quit = 0; while( quit == 0 ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = 1; } }

	return (0);
}
