// Animal.h
#ifndef ANIMAL_H
#define ANIMAL_H
#include <iostream>
class Animal
{
  public:
	std::string food_;
	std::string getType();
	std::string getFood();
	void setType(std::string);
	void setFood(std::string);
	virtual std::string print();
	Animal();
	Animal(std::string, std::string);

  private:
	std::string type_;
};
#endif