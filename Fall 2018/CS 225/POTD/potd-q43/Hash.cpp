#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include "Hash.h"

using namespace std;

unsigned long bernstein(std::string str, int M)
{
	unsigned long b_hash = 5381;
	for (int i = 0; i < (int)str.length(); ++i)
	{
		b_hash = b_hash * 33 + str[i];
	}
	return b_hash % M;
}

float hash_goodness(std::string str, int M)
{
	// std::vector<int> *array = new std::vector<int>(M); // Hint: This comes in handy
	int permutation_count = 0;
	float goodness = 0;
	float collisions = 0;
	vector<int> result;
	do
	{
		if (permutation_count == M)
			break;
		int hash = bernstein(str, M);
		auto it = find(result.begin(), result.end(), hash);
		if (it != result.end())
		{
			collisions++;
		}
		else
		{
			result.push_back(bernstein(str, M));
		}
		permutation_count++;
	} while (std::next_permutation(str.begin(), str.end()));
	goodness = collisions / M;
	return goodness;
}
