#include "Hash.h"
#include <string>
#include <iostream>

unsigned long bernstein(std::string str, int M)
{
	unsigned long b_hash = 5381;
	for (char c : str)
	{
		b_hash *= 33;
		b_hash += int(c);
	}
	return b_hash % M;
}

std::string reverse(std::string str)
{
	std::string output = "";
	for (size_t i = str.size(); i > 0; i--)
	{
		output = output + str[i - 1];
	}
	return output;
}
