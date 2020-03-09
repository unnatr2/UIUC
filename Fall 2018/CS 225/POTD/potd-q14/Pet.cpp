// Pet.cpp
#include "Pet.h"

Pet::Pet() : Animal(), name_("Fluffy"), owner_name_("Cinda") {}
Pet::Pet(string t, string f, string n, string o) : Animal(t, f), name_(n), owner_name_(o) {}
void Pet::setFood(string f) { food_ = f; }
void Pet::setName(string n) { name_ = n; }
void Pet::setOwnerName(string o) { owner_name_ = o; }
string Pet::getName() { return name_; }
string Pet::getFood() { return food_; }
string Pet::getOwnerName() { return owner_name_; }
string Pet::print() { return "My name is " + name_; }