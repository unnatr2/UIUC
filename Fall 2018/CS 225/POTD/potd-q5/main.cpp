#include <iostream>
#include "Food.h"
#include "q5.h"

using namespace std;

int main() {
    Food f;
    cout<<"You have "<<f.get_quantity()<<" "<<f.get_name()<<"."<<endl;
    increase_quantity(&f);
    cout<<"You have "<<f.get_quantity()<<" "<<f.get_name()<<"."<<endl;
    return 1;
}