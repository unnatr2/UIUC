/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <cmath>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim> &first,
                                const Point<Dim> &second, int curDim) const
{
    if (first[curDim] < second[curDim])
    {
        return true;
    }
    else if (first[curDim] > second[curDim])
    {
        return false;
    }
    else
    {
        return first < second;
    }
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim> &target,
                                const Point<Dim> &currentBest,
                                const Point<Dim> &potential) const
{
    int distFromCurrentBest = 0;
    int distFromPotential = 0;
    for (int i = 0; i < Dim; ++i)
    {
        distFromCurrentBest += pow(target[i] - currentBest[i], 2);
        distFromPotential += pow(target[i] - potential[i], 2);
    }
    distFromCurrentBest = sqrt(distFromCurrentBest);
    distFromPotential = sqrt(distFromPotential);
    if (distFromPotential < distFromCurrentBest)
    {
        return true;
    }
    else if (distFromPotential > distFromCurrentBest)
    {
        return false;
    }
    else
    {
        return potential < currentBest;
    }
}

template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>> &Points, int currentDim, int left, int right, int pivotIndex)
{
    Point<Dim> pivot = Points[pivotIndex];
    Point<Dim> temp = Points[right];
    Points[right] = Points[pivotIndex];
    Points[pivotIndex] = temp;
    int leftPointer = left;
    for (int i = left; i < right; i++)
    {
        if (smallerDimVal(Points[i], pivot, currentDim))
        {
            temp = Points[i];
            Points[i] = Points[leftPointer];
            Points[leftPointer] = temp;
            leftPointer++;
        }
    }
    temp = Points[leftPointer];
    Points[leftPointer] = Points[right];
    Points[right] = temp;
    return leftPointer;
}

template <int Dim>
void KDTree<Dim>::quickSelect(vector<Point<Dim>> &Points, int left, int right, int mid, int currentDim)
{
    if (left == right)
    {
        return;
    }
    int pivotIndex = (left + right) / 2;
    pivotIndex = partition(Points, currentDim, left, right, pivotIndex);
    if (mid == pivotIndex)
    {
        return;
    }
    else if (mid < pivotIndex)
    {
        return quickSelect(Points, left, pivotIndex - 1, mid, currentDim);
    }
    else
    {
        return quickSelect(Points, pivotIndex + 1, right, mid, currentDim);
    }
    return;
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode *KDTree<Dim>::makeTree(vector<Point<Dim>> &Points, int left, int right, int currentDim)
{
    if (right < left)
    {
        return NULL;
    }
    KDTreeNode *newNode = new KDTreeNode();
    int mid = (left + right) / 2;
    quickSelect(Points, left, right, mid, currentDim % Dim);
    newNode->point = Points[mid];
    newNode->left = makeTree(Points, left, mid - 1, (currentDim + 1) % Dim);
    newNode->right = makeTree(Points, mid + 1, right, (currentDim + 1) % Dim);
    return newNode;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>> &newPoints)
{
    vector<Point<Dim>> Points(newPoints);
    size = Points.size();
    root = makeTree(Points, 0, Points.size() - 1, 0);
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode *KDTree<Dim>::copy(const KDTreeNode *subRoot)
{
    if (subRoot == NULL)
    {
        return NULL;
    }
    KDTreeNode *newNode = new KDTreeNode(subRoot->point);
    newNode->left = copy(subRoot->left);
    newNode->right = copy(subRoot->right);
    return newNode;
}

template <int Dim>
void KDTree<Dim>::destroy(KDTree<Dim>::KDTreeNode *subRoot)
{
    if (subRoot == NULL)
    {
        return;
    }
    destroy(subRoot->left);
    destroy(subRoot->right);
    delete subRoot;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim> &other)
{
    root = copy(other.root);
    size = other.size;
}

template <int Dim>
const KDTree<Dim> &KDTree<Dim>::operator=(const KDTree<Dim> &rhs)
{
    if (this != &rhs)
    {
        destroy(root);
        root = copy(rhs.root);
        size = rhs.size;
    }
    return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree()
{
    destroy(root);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim> &query) const
{
    KDTreeNode *currentBest = root;
    double minDist = nearestNeighbor(query, root, currentBest, 0);
    return currentBest->point;
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode *KDTree<Dim>::downTree(const Point<Dim> &query, KDTreeNode *node, int currentDim, int &bottom, double &minDist, std::stack<KDTreeNode *> &parents, std::stack<bool> &direction) const
{
    if (query == node->point)
    {
        parents.push(node);
        direction.push(left_);
        minDist = 0;
        bottom = currentDim;
        return node;
    }
    if (smallerDimVal(query, node->point, currentDim))
    {
        parents.push(node);
        direction.push(left_);
        if (node->left)
        {
            return downTree(query, node->left, (currentDim + 1) % Dim, bottom, minDist, parents, direction);
        }
        else
        {
            minDist = distance(query, node->point);
            bottom = currentDim;
            return node;
        }
    }
    else
    {
        parents.push(node);
        direction.push(right_);
        if (node->right)
        {
            return downTree(query, node->right, (currentDim + 1) % Dim, bottom, minDist, parents, direction);
        }
        else
        {
            minDist = distance(query, node->point);
            bottom = currentDim;
            return node;
        }
    }
    return node;
}
template <int Dim>
bool KDTree<Dim>::checkBounds(const Point<Dim> &query, const Point<Dim> &target, int currentDim, double &minDist) const
{
    Point<Dim> bound = query;
    bound.set(currentDim, target[currentDim]);
    if (distance(query, bound) <= minDist)
    {
        return true;
    }
    return false;
}
template <int Dim>
void KDTree<Dim>::upTree(const Point<Dim> &query, KDTreeNode *node, KDTreeNode *&currentBest, int currentDim, double &minDist, std::stack<KDTreeNode *> &parents, std::stack<bool> &direction) const
{
    if (distance(query, node->point) < minDist)
    {
        minDist = distance(query, node->point);
        currentBest = node;
    }
    else if (distance(query, node->point) == minDist)
    {
        if (node->point < currentBest->point)
        {
            currentBest = node;
        }
    }
    bool dir = direction.top();
    parents.pop();
    direction.pop();
    if (checkBounds(query, node->point, currentDim, minDist))
    {
        if (dir && node->right)
        {
            KDTreeNode *tempNode = node;
            double tempDist = nearestNeighbor(query, node->right, tempNode, (currentDim + 1) % Dim);
            if ((tempDist < minDist) || (tempDist == minDist && tempNode->point < currentBest->point))
            {
                currentBest = tempNode;
                minDist = tempDist;
            }
        }
        else if (!dir && node->left)
        {
            KDTreeNode *tempNode = node;
            double tempDist = nearestNeighbor(query, node->left, tempNode, (currentDim + 1) % Dim);
            if ((tempDist < minDist) || (tempDist == minDist && tempNode->point < currentBest->point))
            {
                currentBest = tempNode;
                minDist = tempDist;
            }
        }
    }
    if (!parents.empty())
    {
        KDTreeNode *parent = parents.top();
        upTree(query, parent, currentBest, (currentDim - 1 + Dim) % Dim, minDist, parents, direction);
    }
    return;
}
template <int Dim>
double KDTree<Dim>::nearestNeighbor(const Point<Dim> &query, KDTreeNode *node, KDTreeNode *&currentBest, int currentDim) const
{
    stack<KDTreeNode *> *parents = new stack<KDTreeNode *>();
    stack<bool> *direction = new stack<bool>();
    int *bottom = new int();
    double *minDist = new double();
    KDTreeNode *tempNode = downTree(query, node, currentDim, *bottom, *minDist, *parents, *direction);
    currentBest = tempNode;
    upTree(query, tempNode, currentBest, *bottom, *minDist, *parents, *direction);
    double dist = *minDist;
    delete parents;
    delete direction;
    delete bottom;
    delete minDist;
    return dist;
}

template <int Dim>
double KDTree<Dim>::distance(const Point<Dim> &first, const Point<Dim> &second) const
{
    double result = 0.0;
    for (int i = 0; i < Dim; i++)
    {
        result += pow(first[i] - second[i], 2);
    }
    result = sqrt(result);
    return result;
}