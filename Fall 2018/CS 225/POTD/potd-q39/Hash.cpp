#include <vector>
#include <string>
#include "Hash.h"

using namespace std;

int hashFunction(string s, int M)
{
	// Your Code Here
	//hash function to sum up the ASCII characters of the letters of the string
	int sum = 0;
	for (size_t i = 0; i < s.size(); i++)
	{
		sum += int(s[i]);
	}
	return sum % M;
}

int countCollisions(int M, vector<string> inputs)
{
	int collisions = 0;
	vector<int> hash(M, 0);
	// for (int h = 0; h < M; h++)
	// {
	// 	hash[h] = 0;
	// }
	for (size_t i = 0; i < inputs.size(); i++)
	{
		int hashNumber = hashFunction(inputs[i], M);
		hash[hashNumber]++;
	}
	for (size_t j = 0; j < hash.size(); j++)
	{
		if (hash[j] > 1)
		{
			collisions += hash[j] - 1;
		}
	}
	return collisions;
}
