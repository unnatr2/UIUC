#include "maze.h"
#include <algorithm>
#include <cmath>

SquareMaze::SquareMaze()
{
    width_ = 0;
    height_ = 0;
    size_ = 0;
    cells = DisjointSets();
}

void SquareMaze::makeMaze(int width, int height)
{

    if (width < 2 || height < 2)
    {
        return;
    }
    width_ = width;
    height_ = height;
    size_ = width_ * height_;
    walls = vector<vector<vector<bool>>>(width_);
    for (int row = 0; row < width_; row++)
    {
        walls[row] = vector<vector<bool>>(height_);
        for (int col = 0; col < height_; col++)
        {
            walls[row][col] = vector<bool>(2);
            walls[row][col][0] = true;
            walls[row][col][1] = true;
        }
    }

    cells.addelements(size_);
    int numIndependentTrees = width_ * height_;
    while (numIndependentTrees > 1)
    {
        int randRow = rand() % (width_);
        int randCol = rand() % (height_);
        int randDir = rand() % 2;
        if (walls[randRow][randCol][randDir] == true)
        {
            int index = getIndex(randRow, randCol);
            if (randDir == 1 && randCol < height_ - 1)
            {
                int nextIndex = getIndex(randRow, randCol + 1);
                if (cells.find(index) != cells.find(nextIndex))
                {
                    numIndependentTrees--;
                    cells.setunion(index, nextIndex);
                    walls[randRow][randCol][randDir] = false;
                }
            }
            else if (randDir == 0 && randRow < width_ - 1)
            {
                int nextIndex = getIndex(randRow + 1, randCol);
                if (cells.find(index) != cells.find(nextIndex))
                {
                    numIndependentTrees--;
                    cells.setunion(index, nextIndex);
                    walls[randRow][randCol][randDir] = false;
                }
            }
        }
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const
{
    if (!valid(x, y))
    {
        return false;
    }
    if (dir == 0 && valid(x + 1, y) && walls[x][y][0] == false)
    {
        return true;
    }
    else if (dir == 1 && valid(x, y + 1) && walls[x][y][1] == false)
    {
        return true;
    }
    else if (dir == 2 && valid(x - 1, y) && walls[x - 1][y][0] == false)
    {
        return true;
    }
    else if (dir == 3 && valid(x, y - 1) && walls[x][y - 1][1] == false)
    {
        return true;
    }
    return false;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists)
{
    if (valid(x, y))
    {
        walls[x][y][dir] = exists;
    }
}

vector<int> SquareMaze::solveMaze()
{
    vector<int> path = findShortestPath();
    std::reverse(path.begin(), path.end());
    return path;
}

vector<int> SquareMaze::findShortestPath()
{
    vector<int> distMap = vector<int>(width_ * height_, 0);
    vector<int> parentMap = vector<int>(width_ * height_, 0);
    vector<bool> visited = vector<bool>(width_ * height_, false);
    queue<int> q;
    visited[0] = true;
    q.push(0);
    while (!q.empty())
    {
        int curr = q.front();
        q.pop();
        for (auto &neigh : getAdjacent(curr))
        {
            if (visited[neigh] == false)
            {
                visited[neigh] = true;
                q.push(neigh);
                distMap[neigh] = distMap[curr] + 1;
                parentMap[neigh] = curr;
            }
        }
    }
    int end = findMaxPath(distMap);
    vector<int> path;
    while (parentMap[end] != end)
    {
        int dir = getDirection(parentMap[end], end);
        path.push_back(dir);
        end = parentMap[end];
    }
    return path;
}

vector<int> SquareMaze::getAdjacent(int current)
{
    vector<int> adj;
    int x = getX(current);
    int y = getY(current);
    if (valid(x - 1, y) && !walls[x - 1][y][0])
    {
        adj.push_back(getIndex(x - 1, y));
    }
    if (valid(x + 1, y) && !walls[x][y][0])
    {
        adj.push_back(getIndex(x + 1, y));
    }
    if (valid(x, y - 1) && !walls[x][y - 1][1])
    {
        adj.push_back(getIndex(x, y - 1));
    }
    if (valid(x, y + 1) && !walls[x][y][1])
    {
        adj.push_back(getIndex(x, y + 1));
    }
    return adj;
}

int SquareMaze::findMaxPath(vector<int> &distMap)
{
    int y = height_ - 1;
    int position_max = getIndex(0, y);
    for (int x = 0; x < width_; x++)
    {
        int position = getIndex(x, y);
        if (distMap[position] > distMap[position_max])
        {
            position_max = position;
        }
    }
    return position_max;
}

int SquareMaze::getDirection(int parent, int current)
{
    int parentX = getX(parent);
    int parentY = getY(parent);
    int x = getX(current);
    int y = getY(current);
    if (parentX == x - 1)
    {
        return 0;
    }
    if (parentX == x + 1)
    {
        return 2;
    }
    if (parentY == y - 1)
    {
        return 1;
    }
    if (parentY == y + 1)
    {
        return 3;
    }
    return int();
}

int SquareMaze::getNext(int x, int y, int dir)
{
    if (dir == 0)
    {
        if (valid(x + 1, y))
        {
            return getIndex(x + 1, y);
        }
    }
    if (dir == 1)
    {
        if (valid(x, y + 1))
        {
            return getIndex(x, y + 1);
        }
    }
    if (dir == 2)
    {
        if (valid(x - 1, y))
        {
            return getIndex(x - 1, y);
        }
    }
    if (dir == 3)
    {
        if (valid(x, y - 1))
        {
            return getIndex(x, y - 1);
        }
    }
    return 0;
}

PNG *SquareMaze::drawMaze() const
{
    PNG *result = new PNG(10 * width_ + 1, 10 * height_ + 1);
    for (int i = 0; i < 10 * height_ + 1; i++)
    {

        (result->getPixel(0, i)).h = 0;
        (result->getPixel(0, i)).s = 0;
        (result->getPixel(0, i)).l = 0;
    }
    for (int i = 0; i < 10 * width_ + 1; i++)
    {
        if (i < 1 || i > 9)
        {
            (result->getPixel(i, 0)).h = 0;
            (result->getPixel(i, 0)).s = 0;
            (result->getPixel(i, 0)).l = 0;
        }
    }
    for (int x = 0; x < width_; x++)
    {
        for (int y = 0; y < height_; y++)
        {
            if (walls[x][y][0])
            {
                for (int k = 0; k <= 10; k++)
                {
                    HSLAPixel &pixel = result->getPixel((x + 1) * 10, y * 10 + k);
                    pixel.h = 0;
                    pixel.s = 0;
                    pixel.l = 0;
                }
            }
            if (walls[x][y][1])
            {
                for (int k = 0; k <= 10; k++)
                {
                    HSLAPixel &pixel = result->getPixel(x * 10 + k, (y + 1) * 10);
                    pixel.h = 0;
                    pixel.s = 0;
                    pixel.l = 0;
                }
            }
        }
    }
    return result;
}

pair<int, int> SquareMaze::function(int x, int y) const
{
    pair<int, int> ret(x, y);
    int image_width2 = width_ * 10 + 1;
    int image_height2 = height_ * 10 + 1;
    int image_width = 2 * image_width2;
    int image_height = 1 * image_height2;
    float pi = 3.1415926;

    float r = y + image_height2 / 2;
    float degree = 4 * image_width2 / (2 * pi * image_height2);
    float degree_left = (pi - degree) / 2;
    float degree_y = degree_left + 4 * x / (2 * pi * image_height2);
    ret.first = 2 * r * cos(degree_y) + image_width / 2;
    ret.second = r * sin(degree_y);
    return ret;
}

PNG *SquareMaze::drawMazeCreative() const
{

    int image_width2 = width_ * 10 + 1;
    int image_height2 = height_ * 10 + 1;
    int image_width = 2 * image_width2;
    int image_height = 1.5 * image_height2;
    float pi = 3.1415926;

    float r = image_height2 / 3;
    PNG *mazeImage = new PNG(image_width, image_height);

    HSLAPixel black(0, 0, 0);

    for (int i = 0; i < image_width; i++)
    {
        if ((i < 1 || i > 9) && i < image_height2)
        {
            HSLAPixel &pixel = mazeImage->getPixel(function(i, 0).first, function(i, 0).second);
            pixel.s = black.s;
            pixel.h = black.h;
            pixel.l = black.l;
        }
    }
    for (int i = 0; i < image_height; i++)
    {
        if (i < image_height2)
        {
            HSLAPixel &pixel = mazeImage->getPixel(function(0, i).first, function(0, i).second);
            pixel.s = black.s;
            pixel.h = black.h;
            pixel.l = black.l;
        }
    }
    for (int x = 0; x < width_; x++)
    {
        for (int y = 0; y < height_; y++)
        {
            if (walls[x][y][0])
            {
                for (int k = 0; k <= 10; k++)
                {
                    HSLAPixel &pixel = mazeImage->getPixel(function((x + 1) * 10, y * 10 + k).first, function((x + 1) * 10, y * 10 + k).second);
                    pixel.s = black.s;
                    pixel.h = black.h;
                    pixel.l = black.l;
                }
            }
            if (walls[x][y][1])
            {
                for (int k = 0; k <= 10; k++)
                {
                    HSLAPixel &pixel = mazeImage->getPixel(function(x * 10 + k, (y + 1) * 10).first, function(x * 10 + k, (y + 1) * 10).second);
                    pixel.s = black.s;
                    pixel.h = black.h;
                    pixel.l = black.l;
                }
            }
        }
    }

    return mazeImage;
}

PNG *SquareMaze::drawMazeWithSolutionCreative()
{
    PNG *mazeImage = drawMazeCreative();

    HSLAPixel red(0, 1, 0.5, 1);

    vector<int> path = solveMaze();
    int x = 5;
    int y = 5;
    int i;
    for (int iter = 0; iter < (int)path.size(); iter++)
    {
        if (path[iter] == 0)
        {
            for (i = 0; i <= 10; i++)
            {
                HSLAPixel &pixel = mazeImage->getPixel(function(x + i, y).first, function(x + i, y).second);
                pixel.s = red.s;
                pixel.h = red.h;
                pixel.l = red.l;
            }
            x += 10;
        }
        else if (path[iter] == 1)
        {
            for (i = 0; i <= 10; i++)
            {
                HSLAPixel &pixel = mazeImage->getPixel(function(x, y + i).first, function(x, y + i).second);
                pixel.s = red.s;
                pixel.h = red.h;
                pixel.l = red.l;
            }
            y += 10;
        }
        else if (path[iter] == 2)
        {
            for (i = 0; i <= 10; i++)
            {
                HSLAPixel &pixel = mazeImage->getPixel(function(x - i, y).first, function(x - i, y).second);
                pixel.s = red.s;
                pixel.h = red.h;
                pixel.l = red.l;
            }
            x -= 10;
        }
        else if (path[iter] == 3)
        {
            for (i = 0; i <= 10; i++)
            {
                HSLAPixel &pixel = mazeImage->getPixel(function(x, y - i).first, function(x, y - i).second);
                pixel.s = red.s;
                pixel.h = red.h;
                pixel.l = red.l;
            }
            y -= 10;
        }
    }
    x -= 5;
    y += 5;
    for (i = 1; i < 10; i++)
    {
        HSLAPixel &pixel = mazeImage->getPixel(function(x + i, y).first, function(x + i, y).second);
        pixel.l = 1;
    }
    return mazeImage;
}

PNG *SquareMaze::drawMazeWithSolution()
{
    PNG *result = drawMaze();
    HSLAPixel red(0, 1, 0.5);
    vector<int> path = solveMaze();
    int x = 5;
    int y = 5;
    int i;
    for (int j = 0; j < (int)path.size(); j++)
    {
        if (path[j] == 0)
        {
            for (i = 0; i <= 10; i++)
            {
                HSLAPixel &pixel = result->getPixel(x + i, y);
                pixel.s = red.s;
                pixel.h = red.h;
                pixel.l = red.l;
            }
            x += 10;
        }
        else if (path[j] == 1)
        {
            for (i = 0; i <= 10; i++)
            {
                HSLAPixel &pixel = result->getPixel(x, y + i);
                pixel.s = red.s;
                pixel.h = red.h;
                pixel.l = red.l;
            }
            y += 10;
        }
        else if (path[j] == 2)
        {
            for (i = 0; i <= 10; i++)
            {
                HSLAPixel &pixel = result->getPixel(x - i, y);
                pixel.s = red.s;
                pixel.h = red.h;
                pixel.l = red.l;
            }
            x -= 10;
        }
        else if (path[j] == 3)
        {
            for (i = 0; i <= 10; i++)
            {
                HSLAPixel &pixel = result->getPixel(x, y - i);
                pixel.s = red.s;
                pixel.h = red.h;
                pixel.l = red.l;
            }
            y -= 10;
        }
    }
    x -= 5;
    y += 5;
    for (i = 1; i < 10; i++)
    {
        HSLAPixel &pixel = result->getPixel(x + i, y);
        pixel.l = 1;
    }
    return result;
}

int SquareMaze::getX(int index)
{
    return index / height_;
}
int SquareMaze::getY(int index)
{
    return index % height_;
}

bool SquareMaze::valid(int x, int y) const
{
    return (x >= 0) && (x < width_) && (y >= 0) && (y < height_);
}

int SquareMaze::getIndex(int x, int y) const
{
    return x * height_ + y;
}
