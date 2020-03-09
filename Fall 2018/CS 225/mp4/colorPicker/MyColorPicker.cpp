#include "../cs225/HSLAPixel.h"
#include "../Point.h"
#include <cmath>

#include "ColorPicker.h"
#include "MyColorPicker.h"

using namespace cs225;

/**
 * Picks the color for pixel (x, y).
 * Using your own algorithm
 */

// HSLAPixel MyColorPicker::getColor(unsigned x, unsigned y)
// {
// 	/* @todo [Part 3] */

// 	in.readFromFile("wolfOrig.png");
// 	HSLAPixel &pixel = in.getPixel(x, y);
// 	// std::cout << pixel.h;
// 	return pixel;
// }

MyColorPicker::MyColorPicker(
	HSLAPixel color1, HSLAPixel color2, Point center, unsigned radius) : color1(color1), color2(color2), center(center), radius(radius) {}

HSLAPixel MyColorPicker::getColor(unsigned x, unsigned y)
{
	double dx = (x - center.x);
	double dy = (y - center.y);
	double lx = dx + int(dx) % 100;
	double ly = dy + int(dy) % 100;
	double d = sqrt((lx * lx) + (ly * ly));
	// double d = sqrt((dx * dx) + (dy * dy));
	double pct = (d / radius);

	if (pct >= 1)
	{
		return color2;
	}

	double h = color1.h - (color1.h * pct) + (color2.h * pct);
	double s = color1.s - (color1.s * pct) + (color2.s * pct);
	double l = color1.l - (color1.l * pct) + (color2.l * pct);

	return HSLAPixel(h, s, l);
}