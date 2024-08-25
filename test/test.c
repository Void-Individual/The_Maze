#include "test.h"

SDL_Texture *loadTexture(SDL_instance *instance,char *path)
{
    SDL_Texture* newTexture = NULL; //The final texture
    SDL_Surface* loadedSurface = IMG_Load(path); //Load image at specified path

    if( loadedSurface == NULL )
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(instance->renderer, loadedSurface);
        if( newTexture == NULL )
            printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

int displayWelcomeImage(SDL_instance *instance)
{
	SDL_Event event; /*Holds events from SDL event queue*/
    SDL_Texture* texture = loadTexture(instance, "../src/images/welcome.png");
    if (texture == NULL) {
        printf("Failed to load welcome image!\n");
        return (2);
    }

    //SDL_RenderClear(renderer);
    SDL_RenderCopy(instance->renderer, texture, NULL, NULL);
    SDL_RenderPresent(instance->renderer);
	int x = 0;
	while (SDL_WaitEvent(&event))
	{
		if (event.type == SDL_QUIT)
			x = 1;
		if (event.key.keysym.sym == SDLK_SPACE)
			x = 2;
		if (x != 0)
			break;
	}

    //SDL_Delay(5000); /*Wait 5 seconds*/
    SDL_DestroyTexture(texture);
	return (x);
}

int init(SDL_instance *instance)
{
	/* Initialize SDL*/
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Unable to Initialize SDL: %s\n", SDL_GetError());
		return (1);
	}

	/*Create a new window*/
	instance->window = SDL_CreateWindow("Void's Maze Game \\o/",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (instance->window == NULL)
	{
		fprintf(stderr, "Create window error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}

	/*Create a new renderer instance linked to the window*/
	instance->renderer = SDL_CreateRenderer(instance->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (instance->renderer == NULL)
	{
		SDL_DestroyWindow(instance->window);
		fprintf(stderr, "Create renderer error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	return (0);
}

int main(void)
{
	SDL_instance *instance = NULL;
	int running = 1;
	int check;
	int imgFlags;

	instance = malloc(sizeof(SDL_instance));
	if (instance == NULL)
	{
		fprintf(2, "Error allocating memory\n");
		free(instance);
		return (1);
	}
	if (init(instance) != 0)
	{
		fprintf(2, "Error, failed to init\n");
		free(instance);
		return (1);
	}

	imgFlags = IMG_INIT_PNG; /*Initialize PNG loading*/
 	if( !( IMG_Init( imgFlags ) & imgFlags ) )
        printf( "PNG image could not initialize! Error: %s\n", IMG_GetError());
	else
		while (1)
		{
			check = displayWelcomeImage(instance);
			if (check == 1)
			{
				running = 0;
				break;
			}
			else if (check == 2)
				break;
		}
	SDL_DestroyRenderer(instance->renderer);
    SDL_DestroyWindow(instance->window);
    SDL_Quit();
	free(instance);
	return (running);
}
