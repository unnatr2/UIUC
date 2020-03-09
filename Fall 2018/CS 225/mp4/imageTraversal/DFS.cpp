#include <iterator>
#include <cmath>

#include <list>
#include <queue>
#include <stack>
#include <vector>

#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"
#include "DFS.h"

/**
 * Initializes a depth-first ImageTraversal on a given `png` image,
 * starting at `start`, and with a given `tolerance`.
 * 
 * @param png The image this DFS is going to traverse
 * @param start The start point of this DFS
 * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
 * it will not be included in this DFS
 */
DFS::DFS(const PNG &png, const Point &start, double tolerance)
{
	/** @todo [Part 1] */
	originalImage = png;
	startPoint = start;
	tol = tolerance;
	st.push(startPoint);
}

/**
 * Returns an iterator for the traversal starting at the first point.
 */
ImageTraversal::Iterator DFS::begin()
{
	/** @todo [Part 1] */
	Iterator *it = new Iterator(originalImage, startPoint, tol, this);
	return *it;
}

/**
 * Returns an iterator for the traversal one past the end of the traversal.
 */
ImageTraversal::Iterator DFS::end()
{
	/** @todo [Part 1] */
	Iterator *it = new Iterator();
	return *it;
}

/**
 * Adds a Point for the traversal to visit at some point in the future.
 */
void DFS::add(const Point &point)
{
	/** @todo [Part 1] */
	st.push(point);
}

/**
 * Removes and returns the current Point in the traversal.
 */
Point DFS::pop()
{
	/** @todo [Part 1] */
	Point temp = st.top();
	st.pop();
	return temp;
}

/**
 * Returns the current Point in the traversal.
 */
Point DFS::peek() const
{
	/** @todo [Part 1] */
	return st.top();
}

/**
 * Returns true if the traversal is empty.
 */
bool DFS::empty() const
{
	/** @todo [Part 1] */
	return st.empty();
}
