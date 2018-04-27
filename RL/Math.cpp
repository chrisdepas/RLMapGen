#include "stdafx.h"
#include "Math.h"

#define MY_PI 3.14159265f

float atan2pi(float o, float a) {
	//90, 180, 270, 360 degrees
	if (o == 0 && a >= 0)
		return 0.0f;
	if (o == 0 && a < 0)
		return MY_PI;
	if (a == 0 && o > 0)
		return MY_PI / 2;
	if (a == 0 && o < 0)
		return 3 * MY_PI / 2;

	float Angle = atan(o / a);

	//Account for quadrant
	if (a < 0 && o > 0)
		Angle += MY_PI; // (tan will be negative)
	if (a < 0 && o < 0)
		Angle += MY_PI;
	if (a > 0 && o < 0)
		Angle += 2 * MY_PI;

	return Angle;
}