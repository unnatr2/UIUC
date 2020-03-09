// Pet.h
#ifndef _PET_H
#define _PET_H
#include "Animal.h"

class Pet : public Animal
{
  public:
    Pet();
    Pet(string, string, string, string);
    void setFood(string);
    void setName(string);
    void setOwnerName(string);
    string getName();
    string getFood();
    string getOwnerName();
    string print();

  private:
    string name_;
    string owner_name_;
};
#endif