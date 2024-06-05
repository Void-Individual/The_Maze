#include "maze.h"

/**
 * drawRays - Function to draw the 2D pov rays on the map and
 * the entire 3D environment
 * Return: Nothing
*/

void drawRays(SDL_Instance *instance)
{
	int r, mx, my, mp, dof;
	float vx, vy, rx, ry, ra, xo, yo, disV, disH;

	ra = FixAng(instance->pa + 30); /*Set the ray back 30 degrees*/
	for (r = 0; r < 120; r++) /*Doubled the no of rays to increase fov*/
	{
		int vmt = 0, hmt = 0; /*Vertical and horizontal map textures*/

		/*Check Vertical lines*/
		dof = 0;
		disV = 100000;
		float Tan = tan(degToRad(ra));

		if (cos(degToRad(ra)) > 0.001) /*Ray is looking left*/
		{
			rx = (((int)instance->px >> 6) << 6) + 64;
			ry = (instance->px - rx) * Tan + instance->py;
			xo = 64;
			yo = -xo * Tan;
		}
		else if (cos(degToRad(ra)) < -0.001) /*Ray is looking right*/
		{
			rx = (((int)instance->px >> 6) << 6) - 0.0001;
			ry = (instance->px - rx) * Tan + instance->py;
			xo = -64;
			yo = -xo * Tan;
		}
		else /* Ray is looking up or down and hit no walls*/
		{
			rx = instance->px;
			ry = instance->py;
			dof = 8;
		}

		while (dof < 8)
		{
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;
			mp = my * mapX + mx;
			if (mp > 0 && mp < mapX * mapY && instance->mapW[mp] > 0)
			{ /*A vertical wall has been hit*/
				vmt = instance->mapW[mp] - 1;
				dof = 8;
				disV = cos(degToRad(ra)) * (rx - instance->px) - sin(degToRad(ra)) * (ry - instance->py);
			}
			else /*Check the next vertical*/
			{
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}
		vx = rx;
		vy = ry;

		/*Check Horizontal lines*/
		dof = 0;
		disH = 100000;
		Tan = 1.0 / Tan;
		if (sin(degToRad(ra)) > 0.001) /*The ray is looking up*/
		{
			ry = (((int)instance->py >> 6) << 6) - 0.0001;
			rx = (instance->py - ry) * Tan + instance->px;
			yo = -64;
			xo = -yo * Tan;
		}
		else if (sin(degToRad(ra)) < -0.001) /*The ray is looking down*/
		{
			ry = (((int)instance->py >> 6) << 6) + 64;
			rx = (instance->py - ry) * Tan + instance->px;
			yo = 64;
			xo = -yo * Tan;
		}
		else /*The ray is looking straight left or right*/
		{
			rx = instance->px;
			ry = instance->py;
			dof = 8;
		}

		while (dof < 8)
		{
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;
			mp = my * mapX + mx;
			if (mp > 0 && mp < mapX * mapY && instance->mapW[mp] > 0)
			{ /*A horizontal wall has been hit*/
				hmt = instance->mapW[mp] - 1;
				dof = 8;
				disH = cos(degToRad(ra)) * (rx - instance->px) - sin(degToRad(ra)) * (ry - instance->py);
			}
			else /*Check next horizontal*/
			{
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}
		/*----------Draw the 2D rays-----------------------------------------*/
		float shade = 1;

		SDL_SetRenderDrawColorF(instance, 0, 0.8, 0, 1);
		if (disV < disH) /*If a horizontal hit first, change these values*/
		{
			hmt = vmt;
			shade = 0.5;
			rx = vx;
			ry = vy;
			disH = disV;
			SDL_SetRenderDrawColorF(instance, 0, 0.6, 0, 1);
		}
		/*We dont need the 2D rays anymore*/
		/*SDL_RenderDrawThickLine(instance->px, instance->py, rx, ry, 2);*/
		/*-------------------------------------------------------------------*/

		/*---------------Draw the 3D map-------------------------------------*/
		int ca = FixAng(instance->pa - ra);

		disH = disH * cos(degToRad(ca)); /*To fix fisheye effects*/

		int lineH = (mapS * 640) / disH; /*Line height*/
		float ty_step = 32.0 / (float)lineH;
		float ty_off = 0;

		if (lineH > 640) /*Monitor the line height and limit it*/
		{
			ty_off = (lineH - 640) / 2.0;
			lineH = 640;
		}
		int lineO = 320 - (lineH >> 1); /*Line offset*/

		instance->depth[r] = disH; /*Save this lines depth*/

		/*----------Draw Walls------------------*/
		int y;
		float ty = ty_off * ty_step;
		float tx;

		if (shade == 1) /*For the North and south walls*/
		{
			tx = (int)(rx / 2.0) % 32;
			if (ra > 180) /*Rotate the texture if the map is looking south*/
				tx = 31 - tx;
		}
		else /*For the east and west walls*/
		{
			tx = (int)(ry / 2.0) % 32;
			if (ra > 90 && ra < 270) /*Rotate the texture if the map is looking east*/
				tx = 31 - tx;
		}

		for (y = 0; y < lineH; y++)
		{
			setTextureColor(instance, hmt, shade, tx, ty, 1, 0, 0, 0);
			SDL_RenderDrawLargePoint(instance, 8, r * 8, y + lineO);
			ty += ty_step;
		}
		/*---------Draw floors-------*/
		for (y = lineO + lineH; y < 640; y++)
		{
			float dy = y - (640 / 2.0);
			float deg = degToRad(ra);
			float raFix = cos(degToRad(FixAng(instance->pa - ra)));

			/*Added the *2 into tx and ty to prevent sliding*/
			tx = instance->px / 2 + cos(deg) * 158 * 2 * 32 / dy / raFix;
			ty = instance->py / 2 - sin(deg) * 158 * 2 * 32 / dy / raFix;
			int mp = instance->mapF[(int)(ty / 32.0) * mapX + (int)(tx / 32.0)] * 32 * 32;

			setTextureColor(instance, hmt, shade, tx, ty, 0, 1, 0, mp);
			SDL_RenderDrawLargePoint(instance, 8, r * 8, y);

			/*-----Draw Ceiling------*/
			mp = instance->mapC[(int)(ty / 32.0) * mapX + (int)(tx / 32.0)] * 32 * 32;
			if (mp > 0) /*Only draw the ceiling if it has a value*/
			{
				setTextureColor(instance, hmt, shade, tx, ty, 0, 0, 1, mp);
				SDL_RenderDrawLargePoint(instance, 8, r * 8, 640 - y);
			}
		}
		/*-------------------------------------------------------------------*/
		ra = FixAng(ra - 0.5); /*Move each ray back by half an angle now*/
	}

}
