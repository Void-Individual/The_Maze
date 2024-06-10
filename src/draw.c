#include "maze.h"

/**
 * drawMap2D - Function to draw the 2D map of the game
 * Return: Nothing
*/

void drawMap2D(void)
{
	int x, y, xo, yo;

	for (y = 0; y < mapY; y++)
		for (x = 0; x < mapX; x++)
		{
			xo = x * mapS;
			yo = y * mapS;
			if (mapW[y * mapX + x] > 0)
			{
				int a, b;
				/*Adjust index to match texture array*/
				int ty = (mapW[y * mapX + x] - 1) * 32 * 32 * 3;

				for (a = 0; a < 64; a+=2)
					for (b = 0; b < 64; b+=2)
					{
						int pixel = (((a/2) % 32) * 32 + ((b/2) % 32))*3 + ty; // Repeat texture
						int red = map_textures[pixel+0];
						int green = map_textures[pixel+1];
						int blue = map_textures[pixel+2];
						SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
						SDL_Rect rect = {xo+b, yo+a, 2, 2};
            		    SDL_RenderFillRect(renderer, &rect);
					}
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 165, 42, 42, 255); // Brown color
				SDL_Rect rect = {xo, yo, mapS-1, mapS-1};
                //SDL_RenderFillRect(renderer, &rect);
                drawRectOutline(renderer, rect);
			}
		}
}

/**
 * drawSky - Function to draw the sky
 * Return: Nothing
*/


void drawSky(void)
{
	int x, y;
	for (y = 0; y < 40; y++) /*Because we never see below the horizon, we only need the top half*/
		for (x = 0; x < 120; x++)
		{
			/*To allow the sky to rotate as you turn, use player angle instead*/
			int xo = (int)pa*2 - x;
			if (xo < 0)
				xo += 120;
			xo = xo % 120;
			int pixel = (y*120+xo)*3;
			int red = sky[pixel+0];
			int green = sky[pixel+1];
			int blue = sky[pixel+2];
			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
			SDL_RenderDrawLargePoint(8, x*8, y*8);
		}
}

/**
 * drawScreen - Function to draw full screen pages
 * @v: Version of the screen to draw
 * Return: Nothing
*/

void drawScreen(int v)
{
	int x, y;
	int *T;
	if (v == 1)
		T = start;
	if (v == 2)
		T = win;
	if (v == 3)
		T = lose;
	for (y = 0; y < 80; y++)
		for (x = 0; x < 120; x++)
		{
			int pixel = (y*120+x)*3;
			int red = T[pixel+0]*fade;
			int green = T[pixel+1]*fade;
			int blue = T[pixel+2]*fade;
			SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
			SDL_RenderDrawLargePoint(8, x*8, y*8);
		}
	/*Use fade value to slowly the intensity of display*/
	if (fade < 1)
		fade += 0.005*(fps/15);
	if (fade > 1)
		fade = 1;
//	printf("fade: %f, fps: %f\n", fade, fps);
}

void drawSprite(void)
{
	/*Make this sprite a key*/
	if (px < sp[0].x+30 && px > sp[0].x-30 && py < sp[0].y+30 && py > sp[0].y-30)
	sp[0].state = 0; /*If youre close to its position, pick it up*/
	/*If enemy comes close to player*/
	if (px < sp[3].x+30 && px > sp[3].x-30 && py < sp[3].y+30 && py > sp[3].y-30)
	{/*Lose the game and restart*/
		fade = 0;
		timer = 0;
		gameState = 4;
	}

	/*Enemy attack*/
	/*Normal grid position of sprite*/
	int spx = (int)sp[3].x >> 6, spy = (int)sp[3].y >> 6;
	/*normal grid position plus offset*/
	int spx_add = ((int)sp[3].x + 15) >> 6, spy_add = ((int)sp[3].y + 15) >> 6;
	/*normal grid position minus offset*/
	int spx_sub = ((int)sp[3].x - 15) >> 6, spy_sub = ((int)sp[3].y - 15) >> 6;

	/*Stop the enemy from walking through walls*/
	if (sp[3].x > px && (mapW[spy * 8 + spx_sub] == 0 || mapW[spy * 8 + spx_sub] == temp_door)) /*If the player is to the left*/
		sp[3].x -= 0.03 * fps; /*Move to the left*/
	if (sp[3].x < px && (mapW[spy * 8 + spx_add] == 0|| mapW[spy * 8 + spx_add] == temp_door)) /*If the player is to the right*/
		sp[3].x += 0.03 * fps; /*Move to the right*/
	if (sp[3].y < py && (mapW[spy_add * 8 + spx] == 0 || mapW[spy_add * 8 + spx] == temp_door)) /*If the player is below the enemy*/
		sp[3].y += 0.03 * fps; /*Move downward*/
	if (sp[3].y > py && (mapW[spy_sub * 8 + spx] == 0 || mapW[spy_sub * 8 + spx] == temp_door)) /*If the player is above the enemy*/
		sp[3].y -= 0.03 * fps; /*Move to the left*/
	int x, y, s;
	for (s = 0; s < 4; s++) /*to cycle through all 4 sprites*/
	{
		/*This is done using a 3D projection matrix*/
		float sx = sp[s].x - px; /*Temp float variables*/
		float sy = sp[s].y - py;
		float sz = sp[s].z;

		/*Rotate the sprite around its origin*/
		float CS = cos(degToRad(pa)); /*sine and cosine rotation matrix*/
		float SN = sin(degToRad(pa));
		float a = sy * CS + sx * SN;
		float b = sx * CS - sy * SN;
		sx = a;
		sy = b;

		/* Prevent division by zero or near-zero values */
    	if (sy == 0)
			sy = 0.001;

		/*Convert it to screen x, y*/
		sx = (sx * 108.0/sy) + (120/2);
		sy = (sz * 108.0/sy) + (80/2);



		int x, y, scale= 32 * 80 / b; /*Variables to draw a line*/

		/* Check if the scale is valid */
    	if (scale < 0)
			scale = 0;
		if (scale > 120)
			scale = 120;

		/*Textures*/
		float t_x = 0, t_y = 31, t_y_step = 32.0 / (float)scale;
		float t_x_step = 31.5 / (float)scale;
		/*Draw a point*/
		for (x = sx - scale / 2; x < sx + scale / 2; x++)
		{
			t_y = 31;
			for (y = 0; y < scale; y++)
			{
				if (sp[s].state == 1 && x > 0 && x < 120 && b < depth[x])
				{
					int *Texture = spriteTexture(sp[s].type);
					int pixel = ((int)t_y*32 + (int)t_x)*3;// + (hmt*32*32*3); /*Add hmt to retrieve the right texture per tile*/
					int red = Texture[pixel+0];
					int green = Texture[pixel+1];
					int blue = Texture[pixel+2];
					if (red != 255, green != 0, blue != 255) /*Dont draw if purple*/
     				{
						SDL_SetRenderDrawColor(renderer, red, green, blue, 225);
						/*Only draw this sprite if its onscreen and if its depth is closer than the wall*/
						SDL_RenderDrawLargePoint(8, x * 8, sy * 8 - y * 8);
					}
					t_y -= t_y_step;
					if (t_y < 0)
						t_y = 0;
				}
			}
			t_x += t_x_step;
		}
	}
}

/**
 * drawPlayer - Function to draw the player on the 2D map
 * Return: Nothing
*/

void drawPlayer(void)
{
	/*To prevent player from walking out of the screen*/
	if (px < 1)		px = 1;
	if (py < 1)		py = 1;
	if (px > SCREEN_WIDTH)		px = SCREEN_WIDTH - 1;
	if (py > SCREEN_HEIGHT)		py = SCREEN_HEIGHT - 1;

	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_RenderDrawLargePoint(8, px, py);

	/*Small directional pointer*/
	//SDL_RenderDrawLine(renderer, px, py, px+pdx*10, py+pdy*10);
}
