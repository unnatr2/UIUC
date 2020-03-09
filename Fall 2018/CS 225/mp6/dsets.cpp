#include "dsets.h"

DisjointSets::DisjointSets() {}

DisjointSets::DisjointSets(int num)
{
    elements = vector<int>(num);
    for (int i = 0; i < num; i++)
    {
        elements[i] = -1;
    }
}

void DisjointSets::addelements(int num)
{
    for (int i = 0; i < num; i++)
    {
        elements.push_back(-1);
    }
}

int DisjointSets::find(int elem)
{
    if (elements[elem] < 0)
    {
        return elem;
    }
    else
    {
        int root = find(elements[elem]);
        elements[elem] = root;
        return root;
    }
}

void DisjointSets::setunion(int a, int b)
{
    int rootA = find(a);
    int rootB = find(b);
    int sizeA = -1 * elements[rootA];
    int sizeB = -1 * elements[rootB];
    int newSize = sizeA + sizeB;
    if (sizeA > sizeB)
    {
        elements[rootB] = rootA;
        elements[rootA] = -1 * newSize;
    }
    else
    {
        elements[rootA] = rootB;
        elements[rootB] = -1 * newSize;
    }
}

int DisjointSets::size(int elem)
{
    int root = find(elem);
    return -1 * elements[root];
}