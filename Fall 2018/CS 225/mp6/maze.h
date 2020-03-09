#ifndef _MAZE_H_
#define _MAZE_H_

#include <queue>
#include <stack>
#include <vector>
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include "dsets.h"
using namespace std;
using namespace cs225;

class SquareMaze
{
public:
  SquareMaze();
  void makeMaze(int width, int height);
  bool canTravel(int x, int y, int dir) const;
  void setWall(int x, int y, int dir, bool exists);
  vector<int> solveMaze();
  PNG *drawMaze() const;
  PNG *drawMazeWithSolution();
  PNG *drawMazeCreative() const;
  PNG *drawMazeWithSolutionCreative();

private:
  int size_;
  int width_;
  int height_;
  vector<int> sol;
  DisjointSets cells;
  vector<vector<vector<bool>>> walls;
  vector<int> findShortestPath();
  vector<int> getAdjacent(int current);
  int findMaxPath(vector<int> &distMap);
  int getDirection(int parent, int current);
  pair<int, int> function(int x, int y) const;

  bool valid(int x, int y) const;
  int getIndex(int x, int y) const;
  int getX(int index);
  int getY(int index);
  int getNext(int x, int y, int dir);
};

#endif