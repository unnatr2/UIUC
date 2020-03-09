#include <iostream>
#include "potd.cpp"

using namespace std;

int main()
{
    unordered_map<string, int> mapA({{"common", 13},
                                     {"unique_a", 13}});
    unordered_map<string, int> mapB({{"common", 17},
                                     {"unique_b", 17}});

    unordered_map<string, int> common = common_elems(mapA, mapB);

    // check your outputs here
    for (pair<string, int> pairA : mapA)
    {
        cout << pairA.first << " -> " << pairA.second << endl;
    }
    cout << endl;
    for (pair<string, int> pairB : mapB)
    {
        cout << pairB.first << " -> " << pairB.second << endl;
    }
    cout << endl;
    for (pair<string, int> pairC : common)
    {
        cout << pairC.first << " -> " << pairC.second << endl;
    }
}
