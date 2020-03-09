/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"
//#include "cs225/RGB_HSL.h"

using namespace std;

Point<3> convertToXYZ(LUVAPixel pixel)
{
    return Point<3>(pixel.l, pixel.u, pixel.v);
}

MosaicCanvas *mapTiles(SourceImage const &theSource,
                       vector<TileImage> &theTiles)
{
    vector<Point<3>> points;
    map<Point<3>, int> pointMap;
    for (size_t i = 0; i < theTiles.size(); i++)
    {
        LUVAPixel color = theTiles[i].getAverageColor();
        Point<3> newPoint = Point<3>(color.l, color.u, color.v);
        points.push_back(newPoint);
        pointMap[points[i]] = i;
    }
    KDTree<3> mosaicTree(points);
    MosaicCanvas *mosaic = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
    for (int i = 0; i < mosaic->getRows(); i++)
    {
        for (int j = 0; j < mosaic->getColumns(); j++)
        {
            LUVAPixel color = theSource.getRegionColor(i, j);
            Point<3> point = convertToXYZ(color);
            Point<3> bestMatch = mosaicTree.findNearestNeighbor(point);
            int idx = pointMap[bestMatch];
            mosaic->setTile(i, j, &theTiles[idx]);
        }
    }
    return mosaic;
}
