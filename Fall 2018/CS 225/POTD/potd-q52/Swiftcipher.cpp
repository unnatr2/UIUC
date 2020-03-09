#include "Swiftcipher.h"
#include <string>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

/* Swift Cipher: Frequency of words in a file corresponds to its location in the decrypted sentence */
string decipherer(string filename)
{
	ifstream file(filename);
	string word;
	map<string, int> dict;
	// file.open(filename);
	string result;
	while (file >> word)
	{
		// cout << word << endl;
		int flag = 0;
		for (auto it = dict.begin(); it != dict.end(); it++)
		{
			if (it->first == word)
			{
				flag = 1;
				it->second++;
			}
		}
		// cout << dict["225"] << endl;
		if (flag == 0)
		{
			dict[word] = 1;
		}
	}
	int max = 0;
	for (auto it = dict.begin(); it != dict.end(); it++)
	{
		if (it->second > max)
		{
			max = it->second;
		}
	}
	// cout << max << endl;
	for (int i = 1; i <= max; i++)
	{
		for (auto it = dict.begin(); it != dict.end(); it++)
		{
			if (it->second == i)
			{
				result += it->first;
				result += " ";
			}
		}
	}
	result.pop_back();
	file.close();
	return result;
}
