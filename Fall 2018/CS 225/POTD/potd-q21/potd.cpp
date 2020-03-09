#include "potd.h"

double sum(vector<double>::iterator start, vector<double>::iterator end)
{
    double sum = 0;
    for (vector<double>::iterator it = start; it != end; it++)
    {
        sum += *it;
    }
    return sum;
}

vector<double>::iterator max_iter(vector<double>::iterator start, vector<double>::iterator end)
{
    vector<double>::iterator max = start;
    for (vector<double>::iterator it = start; it != end; it++)
    {
        if (*it > *max)
        {
            max = it;
        }
    }
    return max;
}

void sort_vector(vector<double>::iterator start, vector<double>::iterator end)
{
    for (vector<double>::iterator i = start; i != end; i++)
    {
        vector<double>::iterator max = max_iter(i, end);
        double temp = *i;
        *i = *max;
        *max = temp;
    }
}