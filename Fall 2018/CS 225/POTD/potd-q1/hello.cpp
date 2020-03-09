#include "hello.h"
#include <string>

std::string name ("Unnat");
int age = 19;

std::string hello(){
    std::string output="Hello world! My name is "+ name +" and I am "+ std::to_string(age) +" years old.";
    return output;
}
