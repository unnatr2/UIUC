// Base.h
#ifndef BASE_H
#define BASE_H
#include <iostream>

class Base
{
  public:
    std::string foo();
    virtual std::string bar();
    virtual ~Base();
};

#endif