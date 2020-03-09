// Derived.cpp
#include "Derived.h"
#include <iostream>

std::string Derived::foo()
{
    return "I will not eat them.";
}
std::string Derived::bar()
{
    return "And Ham";
}
Derived::~Derived()
{
}