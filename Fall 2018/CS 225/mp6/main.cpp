#include <iostream>
#include "dsets.h"
#include "maze.h"
#include "cs225/PNG.h"

using namespace std;

int main()
{
    // Write your own main here
    SquareMaze maze;
    maze.makeMaze(50, 50);
    PNG *actualOutput = maze.drawMazeCreative();
    actualOutput->writeToFile("creative" + string(".png"));
    PNG *actualOutput_sol = maze.drawMazeWithSolutionCreative();
    actualOutput_sol->writeToFile("creative_solution" + string(".png"));
    return 0;
}
