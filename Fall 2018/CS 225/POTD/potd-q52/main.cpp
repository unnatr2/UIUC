#include <iostream>
#include <vector>
#include <string>
#include "Swiftcipher.h"

int main()
{
    std::string result = decipherer("example.txt");
    std::cout << result << std::endl;
    return 0;
}
