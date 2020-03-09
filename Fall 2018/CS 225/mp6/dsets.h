#ifndef _DSETS_H_
#define _DSETS_H_

#include <vector>
using namespace std;

class DisjointSets
{
  public:
	DisjointSets();
	DisjointSets(int num);
	void addelements(int num);
	int find(int elem);
	void setunion(int a, int b);
	int size(int elem);

  private:
	vector<int> elements;
};

#endif