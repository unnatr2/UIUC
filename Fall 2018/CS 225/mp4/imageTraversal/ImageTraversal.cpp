#include <cmath>
#include <iterator>
#include <iostream>

#include "../cs225/HSLAPixel.h"
#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"

/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 * 
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
double ImageTraversal::calculateDelta(const HSLAPixel &p1, const HSLAPixel &p2)
{
	double h = fabs(p1.h - p2.h);
	double s = p1.s - p2.s;
	double l = p1.l - p2.l;

	// Handle the case where we found the bigger angle between two hues:
	if (h > 180)
	{
		h = 360 - h;
	}
	h /= 360;

	return sqrt((h * h) + (s * s) + (l * l));
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator()
{
	/** @todo [Part 1] */
}

ImageTraversal::Iterator::Iterator(PNG &png, Point &point, double tolerance, ImageTraversal *next)
{
	traversal = next;
	image = png;
	startPoint = point;
	currentPoint = point;
	tol = tolerance;
	std::vector<bool> temp;
	for (unsigned int i = 0; i < image.height(); i++)
	{
		temp.push_back(false);
	}
	for (unsigned int i = 0; i < image.width(); i++)
	{
		visited.push_back(temp);
	}
	visited[startPoint.x][startPoint.y] = true;
	unsigned int height = image.height();
	unsigned int width = image.width();
	HSLAPixel &pixel = image.getPixel(startPoint.x, startPoint.y);
	if (currentPoint.x + 1 < width)
	{
		HSLAPixel &curr = image.getPixel(currentPoint.x + 1, currentPoint.y);
		if (calculateDelta(pixel, curr) < tol)
		{
			traversal->add(Point(currentPoint.x + 1, currentPoint.y));
		}
	}
	if (currentPoint.y + 1 < height)
	{
		HSLAPixel &curr = image.getPixel(currentPoint.x, currentPoint.y + 1);
		if (calculateDelta(pixel, curr) < tol)
		{
			traversal->add(Point(currentPoint.x, currentPoint.y + 1));
		}
	}
	if (int(currentPoint.x) - 1 >= 0)
	{
		HSLAPixel &curr = image.getPixel(currentPoint.x - 1, currentPoint.y);
		if (calculateDelta(pixel, curr) < tol)
		{
			traversal->add(Point(currentPoint.x - 1, currentPoint.y));
		}
	}
	if (int(currentPoint.y) - 1 >= 0)
	{
		HSLAPixel &curr = image.getPixel(currentPoint.x, currentPoint.y - 1);
		if (calculateDelta(pixel, curr) < tol)
		{
			traversal->add(Point(currentPoint.x, currentPoint.y - 1));
		}
	}
}

/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator &ImageTraversal::Iterator::operator++()
{
	/** @todo [Part 1] */
	if (traversal->empty())
	{
		traversal = NULL;
		return *this;
	}
	Point temp = traversal->pop();
	while (visited[temp.x][temp.y])
	{
		if (traversal->empty())
		{
			traversal = NULL;
			return *this;
		}
		temp = traversal->pop();
	}
	visited[temp.x][temp.y] = true;
	currentPoint = temp;
	unsigned int height = image.height();
	unsigned int width = image.width();
	HSLAPixel &pixel = image.getPixel(startPoint.x, startPoint.y);
	if (currentPoint.x + 1 < width)
	{
		HSLAPixel &curr = image.getPixel(currentPoint.x + 1, currentPoint.y);
		if (calculateDelta(pixel, curr) < tol)
		{
			traversal->add(Point(currentPoint.x + 1, currentPoint.y));
		}
	}
	if (currentPoint.y + 1 < height)
	{
		HSLAPixel &curr = image.getPixel(currentPoint.x, currentPoint.y + 1);
		if (calculateDelta(pixel, curr) < tol)
		{
			traversal->add(Point(currentPoint.x, currentPoint.y + 1));
		}
	}
	if (int(currentPoint.x) - 1 >= 0)
	{
		HSLAPixel &curr = image.getPixel(currentPoint.x - 1, currentPoint.y);
		if (calculateDelta(pixel, curr) < tol)
		{
			traversal->add(Point(currentPoint.x - 1, currentPoint.y));
		}
	}
	if (int(currentPoint.y) - 1 >= 0)
	{
		HSLAPixel &curr = image.getPixel(currentPoint.x, currentPoint.y - 1);
		if (calculateDelta(pixel, curr) < tol)
		{
			traversal->add(Point(currentPoint.x, currentPoint.y - 1));
		}
	}
	if (traversal->empty())
	{
		traversal = NULL;
	}
	return *this;
}

/**
 * Iterator accessor opreator.
 * 
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*()
{
	/** @todo [Part 1] */
	return currentPoint;
}

/**
 * Iterator inequality operator.
 * 
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other)
{
	/** @todo [Part 1] */
	return traversal != NULL;
}
