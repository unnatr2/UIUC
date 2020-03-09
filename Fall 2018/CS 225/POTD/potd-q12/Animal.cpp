// Animal.cpp
#include "Animal.h"

std::string Animal::getType()
{
    return type_;
}
std::string Animal::getFood()
{
    return food_;
}
void Animal::setType(std::string t)
{
    type_ = t;
}
void Animal::setFood(std::string f)
{
    food_ = f;
}
std::string Animal::print()
{
    std::string output = "I am a " + type_ + ".";
    return output;
}
Animal::Animal()
{
    type_ = "cat";
    food_ = "fish";
}
Animal::Animal(std::string t, std::string f)
{
    type_ = t;
    food_ = f;
}