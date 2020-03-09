#include <vector>
#include "Hash.h"

void doubleHashInput(std::vector<int> &v, int elem)
{
  int idx = firstHash(elem, v.size());
  int offset = secondHash(elem);
  while (v[idx] != -1)
  {
    idx = (idx + offset) % v.size();
  }
  v[idx] = elem;
}

int firstHash(int elem, int length)
{
  elem *= 4;
  return elem % length;
}
int secondHash(int elem)
{
  elem = elem % 3;
  return 3 - elem;
}
