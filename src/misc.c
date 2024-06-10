#include "maze.h"

/**
 * degToRad - Function to convert values from degrees to radians
 * @a: Degree value
 * Return: The converted value in radian
*/

float degToRad(float a)
{
	return (a * M_PI / 180.0);
}

/**
 * FixAng - Function to balance out any angles going out of bounds
 * @a: angle to be corrected
 * Return: corrected angle
*/

float FixAng(float a)
{
	if (a > 359)
		a -= 360;
	if (a < 0)
		a += 360;

	return (a);
}
