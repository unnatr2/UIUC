#include <unordered_map>
#include <vector>

using namespace std;

unordered_map<string, int> common_elems(unordered_map<string, int> &mapA,
                                        unordered_map<string, int> &mapB)
{

    unordered_map<string, int> result;
    vector<string> v;
    for (unordered_map<string, int>::iterator itA = mapA.begin(); itA != mapA.end(); itA++)
    {
        for (unordered_map<string, int>::iterator itB = mapB.begin(); itB != mapB.end(); itB++)
        {
            if (itA->first == itB->first)
            {
                result[itA->first] = itA->second + itB->second;
                v.push_back(itA->first);
            }
        }
    }
    for (size_t i = 0; i < v.size(); i++)
    {
        // cout << v[i] << endl;
        mapA.erase(v[i]);
        mapB.erase(v[i]);
    }

    return result;
}