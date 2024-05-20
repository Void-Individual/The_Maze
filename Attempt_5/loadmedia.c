#include "maze.h"

/**
 * loadMedia - Function to load media
 * Return: 1 on success, else true
*/

int loadMedia(void)
{
	int success = 1;
    int imgFlags;

    //Load a png texture
    loadedTexture = loadTexture("../image/texture.png");
    if(loadedTexture == NULL )
    {
        printf( "Failed to load texture image!\n" );
        success = 0;
    }
    imgFlags = IMG_INIT_PNG; //Initialize PNG loading
 	if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "PNG image could not initialize! Error: %s\n", IMG_GetError() );
        success = 0;
    }
    else
    {
        gKeyPressSurfaces[KEY_PRESS_SURFACE_p] = loadSurface("../image/texture.png");
	    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_p ] == NULL )
        {
            printf( "Failed to load png image!\n" );
            success = 0;
        }
    }

    /*Load default image*/
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("../image/press.bmp");
	  if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
    {
        printf( "Failed to load default image!\n" );
        success = 0;
    }

    //Load up surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface( "../image/up.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL )
    {
        printf( "Failed to load up image!\n" );
        success = 0;
    }

    //Load down surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface( "../image/down.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL )
    {
        printf( "Failed to load down image!\n" );
        success = 0;
    }

    //Load left surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface( "../image/left.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL )
    {
        printf( "Failed to load left image!\n" );
        success = 0;
    }

    //Load right surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface( "../image/right.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
    {
        printf( "Failed to load right image!\n" );
        success = 0;
    }

	return (success);
}

/**
 * draw_stuff - Function to draw lines on the window
 * Return: Empty
*/

void draw_stuff(void)
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0xFF, 0xFF);
	SDL_RenderDrawLine(renderer, 10, 10, 100, 100);
}
