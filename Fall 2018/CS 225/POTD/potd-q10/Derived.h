// Derived.h
#ifndef DERIVED_H
#define DERIVED_H
#include <iostream>
#include "Base.h"

class Derived : public Base
{
  public:
    std::string foo();
    virtual std::string bar();
    virtual ~Derived();
};

#endif